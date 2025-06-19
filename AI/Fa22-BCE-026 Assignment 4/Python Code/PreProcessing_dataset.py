# !pip install facenet-pytorch
# !pip install mxnet
# !pip install tqdm matplotlib
# Import only what's needed for preprocessing
import os
import numpy as np
import cv2
from tqdm import tqdm
import glob
from PIL import Image
from torch.utils.data import Dataset

# Custom dataset class for loading the preprocessed data
class FaceDataset(Dataset):
    def __init__(self, data_dir, transform=None):
        self.data_dir = data_dir
        self.transform = transform
        self.image_paths = []
        self.labels = []
        self.label_map = {}

        label_idx = 0
        print("Loading dataset...")

        # Process directory structure
        print(f"Processing directory: {data_dir}")
        for identity in tqdm(sorted(os.listdir(data_dir))):
            identity_dir = os.path.join(data_dir, identity)
            if not os.path.isdir(identity_dir):
                continue

            # Get image files
            image_files = glob.glob(os.path.join(identity_dir, "*.jpg"))
            if len(image_files) < 5:  # Filter identities with < 5 images
                continue

            # Assign label
            if identity not in self.label_map:
                self.label_map[identity] = label_idx
                label_idx += 1

            # Limit to 100 images per identity
            for img_path in image_files[:100]:
                self.image_paths.append(img_path)
                self.labels.append(self.label_map[identity])

        print(f"Loaded {len(self.image_paths)} images with {len(self.label_map)} identities")

    def __len__(self):
        return len(self.image_paths)

    def __getitem__(self, idx):
        img_path = self.image_paths[idx]
        label = self.labels[idx]

        try:
            image = Image.open(img_path).convert('RGB')
        except Exception as e:
            print(f"Error opening {img_path}: {e}")
            image = Image.new('RGB', (160, 160), color=(0, 0, 0))

        if self.transform:
            image = self.transform(image)

        return image, label

# Modified process_casia_webface function
def process_casia_webface(dataset_dir, output_dir='/content/datasets/processed_casia'):
    """
    Process the CASIA-WebFace dataset from RecordIO format to individual images if needed
    """
    record_file = os.path.join(dataset_dir, 'train.rec')
    if os.path.exists(record_file):
        print("Found RecordIO format, converting to images...")
        try:
            import numpy as np
            np.bool = bool  # Patch to fix deprecated np.bool issue
            import mxnet as mx
            idx_file = os.path.join(dataset_dir, 'train.idx')
            if not os.path.exists(idx_file):
                print(f"Error: Index file {idx_file} not found")
                return None

            os.makedirs(output_dir, exist_ok=True)
            record = mx.recordio.MXIndexedRecordIO(idx_file, record_file, 'r')
            keys = list(record.keys)

            # Initialize counters
            success_count = 0
            failed_count = 0

            # Process each image with error handling
            for i in tqdm(range(len(keys))):
                key = keys[i]
                try:
                    item = record.read_idx(key)
                    header, img = mx.recordio.unpack_img(item)
                    label = int(header.label)
                    identity_dir = os.path.join(output_dir, str(label))
                    os.makedirs(identity_dir, exist_ok=True)
                    img_path = os.path.join(identity_dir, f"{key}.jpg")
                    if cv2.imwrite(img_path, img):  # Check if write succeeds
                        success_count += 1
                    else:
                        print(f"Failed to write image {key}")
                        failed_count += 1
                except Exception as e:
                    print(f"Error processing image {key}: {e}")
                    failed_count += 1

            # Print summary
            print(f"Conversion complete. {success_count} images saved to {output_dir}, {failed_count} images skipped due to errors.")
            return output_dir
        except Exception as e:
            print(f"Error initializing RecordIO processing: {e}")
            return None
    else:
        print("No RecordIO files found. Assuming images are already extracted.")
        return dataset_dir

# This conditional is crucial when importing this file as a module in train_model.py
if __name__ == "__main__":
    # For standalone testing in Colab
    dataset_dir = "/content/datasets/casia-webface"
    output_dir = "/content/datasets/processed_casia"
    processed_path = process_casia_webface(dataset_dir, output_dir)
    print(f"Processed dataset path: {processed_path}")
