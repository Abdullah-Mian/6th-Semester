
import os
import cv2
import torch
import numpy as np
import pickle
from facenet_pytorch import MTCNN, InceptionResnetV1
from PIL import Image
import argparse

class LocalFaceIDSystem:
        def __init__(self, model_path='models/best_face_model.pth', embedding_path='models/face_embeddings.pkl'):
            """Initialize the Face ID System for local execution"""
            # Create required directories
            os.makedirs("faces", exist_ok=True)
            os.makedirs("models", exist_ok=True)
            
            self.model_path = model_path
            self.embedding_path = embedding_path
            
            self.device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
            print(f"Using device: {self.device}")
            print("Running on local machine")
            
            # Initialize face detection model
            self.mtcnn = MTCNN(
                image_size=160, margin=0, min_face_size=20,
                thresholds=[0.6, 0.7, 0.7], factor=0.709, device=self.device
            )
            
            # Load or initialize face recognition model
            self.resnet = None
            self.load_recognition_model()
            
            # Dictionary for registered faces
            self.registered_faces = self.load_embeddings()
        
        def load_recognition_model(self):
            """
            Load the face recognition model (either trained or pre-trained)
            """
            # First check if there's a trained model available
            if os.path.exists(self.model_path):
                try:
                    print(f"Loading trained model from {self.model_path}")
                    checkpoint = torch.load(self.model_path, map_location=self.device)
                    model = InceptionResnetV1(
                        pretrained='vggface2',
                        classify=False  # We only want the embeddings, not the classification
                    ).to(self.device)
                    
                    # Load the weights but skip the classification layer
                    state_dict = checkpoint['model_state_dict']
                    # Remove classifier weights
                    for key in list(state_dict.keys()):
                        if key.startswith('classifier'):
                            del state_dict[key]
                    
                    model.load_state_dict(state_dict, strict=False)
                    model.eval()
                    self.resnet = model
                    print("Trained model loaded successfully.")
                except Exception as e:
                    print(f"Error loading trained model: {e}")
                    print("Falling back to pre-trained model.")
                    self.resnet = InceptionResnetV1(pretrained='vggface2').eval().to(self.device)
            else:
                print("No trained model found. Using pre-trained model.")
                self.resnet = InceptionResnetV1(pretrained='vggface2').eval().to(self.device)
        
        def load_embeddings(self):
            """Load registered face embeddings if they exist"""
            if os.path.exists(self.embedding_path):
                try:
                    with open(self.embedding_path, 'rb') as f:
                        registered_faces = pickle.load(f)
                    print(f"Loaded {len(registered_faces)} registered faces")
                    return registered_faces
                except Exception as e:
                    print(f"Error loading embeddings: {e}")
            
            print("No registered faces found. Starting with empty database.")
            return {}
        
        def save_embeddings(self):
            """Save registered face embeddings to disk"""
            os.makedirs(os.path.dirname(self.embedding_path), exist_ok=True)
            with open(self.embedding_path, 'wb') as f:
                pickle.dump(self.registered_faces, f)
            print(f"Saved {len(self.registered_faces)} registered faces")
        
        def get_embedding(self, face_img):
            """Get embedding from face image"""
            # Convert to RGB if needed
            if len(face_img.shape) == 2:
                face_img = cv2.cvtColor(face_img, cv2.COLOR_GRAY2RGB)
            elif face_img.shape[2] == 4:
                face_img = cv2.cvtColor(face_img, cv2.COLOR_BGRA2RGB)
            elif face_img.shape[2] == 3:
                face_img = cv2.cvtColor(face_img, cv2.COLOR_BGR2RGB)
            
            # Convert to PIL Image
            face_img = Image.fromarray(face_img)
            
            # Get face embedding
            face_tensor = self.mtcnn(face_img).unsqueeze(0).to(self.device)
            if face_tensor is None:
                raise ValueError("Face detection failed")
                
            with torch.no_grad():
                embedding = self.resnet(face_tensor)
            
            return embedding.cpu().numpy()[0]
        
        def register_face(self):
            """Register a new face using webcam"""
            cap = cv2.VideoCapture(0)
            
            if not cap.isOpened():
                print("Error: Could not open webcam")
                return False
            
            # Get user name
            name = input("Enter name to register: ")
            if not name.strip():
                print("Registration canceled - name cannot be empty")
                cap.release()
                cv2.destroyAllWindows()
                return False
            
            embeddings = []
            count = 0
            max_samples = 5  # Collect multiple samples for better accuracy
            
            print(f"Registering {name}. Please look at the camera...")
            print(f"We'll take {max_samples} samples. Press 'c' to capture each sample.")
            
            while count < max_samples:
                ret, frame = cap.read()
                if not ret:
                    print("Failed to grab frame")
                    break
                    
                # Display the frame
                display_frame = frame.copy()
                cv2.putText(display_frame, f"Sample {count+1}/{max_samples} - Press 'c' to capture", 
                           (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
                
                # Detect faces
                boxes, _ = self.mtcnn.detect(frame)
                if boxes is not None:
                    for box in boxes:
                        box = [int(b) for b in box]
                        cv2.rectangle(display_frame, (box[0], box[1]), (box[2], box[3]), (0, 255, 0), 2)
                
                cv2.imshow('Register Face', display_frame)
                
                key = cv2.waitKey(1) & 0xFF
                if key == ord('q'):
                    break
                elif key == ord('c'):
                    # Try to detect face
                    if boxes is not None and len(boxes) > 0:
                        # Get largest face
                        areas = [(b[2]-b[0])*(b[3]-b[1]) for b in boxes]
                        largest_idx = np.argmax(areas)
                        box = [int(b) for b in boxes[largest_idx]]
                        
                        # Extract face
                        face_img = frame[box[1]:box[3], box[0]:box[2]]
                        
                        try:
                            # Get embedding
                            embedding = self.get_embedding(face_img)
                            embeddings.append(embedding)
                            count += 1
                            
                            # Save face image
                            face_dir = os.path.join("faces", name)
                            os.makedirs(face_dir, exist_ok=True)
                            cv2.imwrite(os.path.join(face_dir, f"sample_{count}.jpg"), face_img)
                            
                            print(f"Sample {count}/{max_samples} captured")
                        except Exception as e:
                            print(f"Error processing face: {e}")
                    else:
                        print("No face detected! Please position your face in the frame.")
            
            cap.release()
            cv2.destroyAllWindows()
            
            if len(embeddings) > 0:
                # Average the embeddings for robustness
                avg_embedding = np.mean(embeddings, axis=0)
                # Normalize the embedding
                avg_embedding = avg_embedding / np.linalg.norm(avg_embedding)
                
                # Store in the dictionary
                self.registered_faces[name] = avg_embedding
                self.save_embeddings()
                
                print(f"Successfully registered {name}")
                return True
            else:
                print(f"Failed to register {name}")
                return False
        
        def verify_face(self):
            """Verify a face against registered faces"""
            if not self.registered_faces:
                print("No faces are registered yet!")
                return
            
            cap = cv2.VideoCapture(0)
            
            if not cap.isOpened():
                print("Error: Could not open webcam")
                return
            
            print("Looking for faces to verify...")
            print("Press 'q' to quit, 'v' to verify current face")
            
            while True:
                ret, frame = cap.read()
                if not ret:
                    print("Failed to grab frame")
                    break
                
                # Create a copy for display
                display_frame = frame.copy()
                
                # Detect faces
                boxes, _ = self.mtcnn.detect(frame)
                if boxes is not None:
                    for box in boxes:
                        box = [int(b) for b in box]
                        cv2.rectangle(display_frame, (box[0], box[1]), (box[2], box[3]), (0, 255, 0), 2)
                
                # Show instructions
                cv2.putText(display_frame, "Press 'v' to verify, 'q' to quit", 
                           (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
                          
                cv2.imshow('Verify Face', display_frame)
                
                key = cv2.waitKey(1) & 0xFF
                if key == ord('q'):
                    break
                elif key == ord('v'):
                    if boxes is not None and len(boxes) > 0:
                        # Get largest face
                        areas = [(b[2]-b[0])*(b[3]-b[1]) for b in boxes]
                        largest_idx = np.argmax(areas)
                        box = [int(b) for b in boxes[largest_idx]]
                        
                        # Extract face
                        face_img = frame[box[1]:box[3], box[0]:box[2]]
                        
                        try:
                            # Get embedding
                            embedding = self.get_embedding(face_img)
                            
                            # Compare with registered faces
                            best_match = None
                            best_similarity = -1
                            threshold = 0.6  # Adjust as needed, higher = stricter
                            
                            for name, reg_embedding in self.registered_faces.items():
                                similarity = np.dot(embedding, reg_embedding)
                                if similarity > best_similarity:
                                    best_similarity = similarity
                                    best_match = name
                            
                            # Display result
                            result_frame = frame.copy()
                            
                            if best_similarity > threshold:
                                # Authentication success
                                cv2.putText(result_frame, f"Welcome, {best_match}!", (10, 50), 
                                           cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                                cv2.putText(result_frame, f"Confidence: {best_similarity:.2f}", (10, 90), 
                                           cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
                                print(f"Authentication successful: {best_match} with confidence {best_similarity:.4f}")
                            else:
                                # Authentication failed
                                cv2.putText(result_frame, "Access Denied", (10, 50), 
                                           cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
                                cv2.putText(result_frame, f"Best match: {best_match} ({best_similarity:.2f})", (10, 90), 
                                           cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
                                print(f"Authentication failed. Best match was {best_match} with confidence {best_similarity:.4f}")
                            

                            cv2.imshow("Verification Result", result_frame)
                            cv2.waitKey(3000)  # Display result for 3 seconds
                            cv2.destroyWindow("Verification Result")
                            
                        except Exception as e:
                            print(f"Error processing face: {e}")
                    else:
                        print("No face detected! Please position your face in the frame.")
            
            cap.release()
            cv2.destroyAllWindows()
        
        def list_registered_faces(self):
            """List all registered faces"""
            if not self.registered_faces:
                print("No faces registered yet")
            else:
                print("\nRegistered faces:")
                for i, name in enumerate(self.registered_faces.keys(), 1):
                    print(f"{i}. {name}")
        
        def delete_registration(self):
            """Delete a registered face"""
            if not self.registered_faces:
                print("No faces registered yet")
                return
                
            print("\nRegistered faces:")
            names = list(self.registered_faces.keys())
            for i, name in enumerate(names, 1):
                print(f"{i}. {name}")
                
            try:
                idx = int(input("\nEnter number to delete (0 to cancel): ")) - 1
                if idx == -1:  # User entered 0 to cancel
                    print("Deletion canceled")
                    return
                    
                name_to_delete = names[idx]
                del self.registered_faces[name_to_delete]
                print(f"Deleted {name_to_delete}")
                
                # Save updated embeddings
                self.save_embeddings()
                
                # Optionally delete face images
                face_dir = os.path.join("faces", name_to_delete)
                if os.path.exists(face_dir):
                    import shutil
                    shutil.rmtree(face_dir)
                    print(f"Deleted face images for {name_to_delete}")
                    
            except (ValueError, IndexError):
                print("Invalid selection")
        
        def run(self):
            """Run the main menu interface"""
            while True:
                print("\n==== Face ID System (Local Mode) ====")
                print("1. Register a new face")
                print("2. Verify a face")
                print("3. List registered faces")
                print("4. Delete a registration")
                print("5. Exit")
                
                choice = input("Enter your choice (1-5): ")
                
                if choice == '1':
                    self.register_face()
                        
                elif choice == '2':
                    self.verify_face()
                    
                elif choice == '3':
                    self.list_registered_faces()
                    
                elif choice == '4':
                    self.delete_registration()
                        
                elif choice == '5':
                    print("Exiting program")
                    break
                    
                else:
                    print("Invalid choice. Please try again.")

def main_local():
        """Main function for local execution"""
        parser = argparse.ArgumentParser(description='Face ID System (Local)')
        parser.add_argument('--model', type=str, default='models/best_face_model.pth', help='Path to trained model')
        parser.add_argument('--embeddings', type=str, default='models/face_embeddings.pkl', help='Path to embeddings file')
        args = parser.parse_args()
        
        face_system = LocalFaceIDSystem(args.model, args.embeddings)
        face_system.run()

if __name__ == "__main__":
    main_local()
