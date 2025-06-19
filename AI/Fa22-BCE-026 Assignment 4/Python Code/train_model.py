# Import training-specific libraries
import os
import torch
import torch.nn as nn
import torch.optim as optim
import torchvision.transforms as transforms
import numpy as np
from torch.utils.data import DataLoader
from facenet_pytorch import InceptionResnetV1
import matplotlib.pyplot as plt
import pickle
from tqdm import tqdm

# Set up GPU
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
print(f"Using device: {device}")

def train_model():
    """
    Train a face recognition model on the preprocessed dataset
    """
    # Settings for training
    data_path = "/content/datasets/casia-webface"
    output_dir = "/content/models"
    batch_size = 16  # Adjusted for GPU memory
    epochs = 1
    lr = 0.0001
    workers = 2  # Number of data loading workers


    # Data transforms
    print("Step 1: Setting up data transformations")
    data_transforms = transforms.Compose([
        transforms.RandomHorizontalFlip(),
        transforms.Resize((160, 160)),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.5, 0.5, 0.5], std=[0.5, 0.5, 0.5])
    ])

    # Create dataset and dataloaders
    print("Step 2: Creating datasets and dataloaders")
    dataset = FaceDataset(processed_path, data_transforms)
    if len(dataset) == 0:
        print("Error: No valid images found!")
        return

    train_size = int(0.8 * len(dataset))
    val_size = len(dataset) - train_size
    train_dataset, val_dataset = torch.utils.data.random_split(dataset, [train_size, val_size])

    train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True, num_workers=workers)
    val_loader = DataLoader(val_dataset, batch_size=batch_size, shuffle=False, num_workers=workers)

    # Initialize model
    print("Step 3: Setting up the model")
    num_classes = len(dataset.label_map)
    print(f"Training with {num_classes} identities")
    model = InceptionResnetV1(pretrained='vggface2', classify=True, num_classes=num_classes).to(device)

    # Loss function and optimizer
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=lr)
    scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)

    # Initialize tracking variables
    best_acc = 0.0
    train_losses, val_losses, train_accs, val_accs = [], [], [], []
    os.makedirs(output_dir, exist_ok=True)

    # Training loop
    print("Step 4: Starting training")
    for epoch in range(epochs):
        print(f'Epoch {epoch+1}/{epochs}')

        # Training phase
        model.train()
        running_loss = 0.0
        running_corrects = 0
        for inputs, labels in tqdm(train_loader, desc=f"Training epoch {epoch+1}/{epochs}"):
            inputs, labels = inputs.to(device), labels.to(device)
            optimizer.zero_grad()
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()
            running_loss += loss.item() * inputs.size(0)
            _, preds = torch.max(outputs, 1)
            running_corrects += torch.sum(preds == labels.data)

        scheduler.step()
        epoch_loss = running_loss / len(train_dataset)
        epoch_acc = running_corrects.float() / len(train_dataset)
        train_losses.append(epoch_loss)
        train_accs.append(epoch_acc.cpu().numpy())
        print(f'Train Loss: {epoch_loss:.4f} Acc: {epoch_acc:.4f}')

        # Validation phase
        model.eval()
        running_loss = 0.0
        running_corrects = 0
        with torch.no_grad():
            for inputs, labels in tqdm(val_loader, desc=f"Validating epoch {epoch+1}/{epochs}"):
                inputs, labels = inputs.to(device), labels.to(device)
                outputs = model(inputs)
                loss = criterion(outputs, labels)
                running_loss += loss.item() * inputs.size(0)
                _, preds = torch.max(outputs, 1)
                running_corrects += torch.sum(preds == labels.data)

        val_epoch_loss = running_loss / len(val_dataset)
        val_epoch_acc = running_corrects.float() / len(val_dataset)
        val_losses.append(val_epoch_loss)
        val_accs.append(val_epoch_acc.cpu().numpy())
        print(f'Val Loss: {val_epoch_loss:.4f} Val Acc: {val_epoch_acc:.4f}')

        # Save best model
        if val_epoch_acc > best_acc:
            best_acc = val_epoch_acc
            torch.save({
                'epoch': epoch,
                'model_state_dict': model.state_dict(),
                'optimizer_state_dict': optimizer.state_dict(),
                'accuracy': val_epoch_acc,
                'num_classes': num_classes
            }, os.path.join(output_dir, 'best_face_model.pth'))
            print(f"Saved best model with accuracy: {best_acc:.4f}")

    # Save final model and label map
    print("Step 5: Saving final model and plotting results")
    torch.save({
        'epoch': epochs,
        'model_state_dict': model.state_dict(),
        'optimizer_state_dict': optimizer.state_dict(),
        'accuracy': best_acc,
        'num_classes': num_classes
    }, os.path.join(output_dir, 'final_face_model.pth'))

    with open(os.path.join(output_dir, 'label_map.pkl'), 'wb') as f:
        pickle.dump(dataset.label_map, f)

    # Plot training history
    plt.figure(figsize=(12, 4))
    plt.subplot(1, 2, 1)
    plt.plot(train_losses, label='Train Loss')
    plt.plot(val_losses, label='Val Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.subplot(1, 2, 2)
    plt.plot(train_accs, label='Train Acc')
    plt.plot(val_accs, label='Val Acc')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, 'training_history.png'))

    print(f"Training complete. Final model saved at {os.path.join(output_dir, 'final_face_model.pth')}")
    print(f"Best accuracy: {best_acc:.4f}")
    print(f"Results saved to {output_dir}")

if __name__ == "__main__":
    train_model()