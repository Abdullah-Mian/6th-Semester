import os
os.environ['KAGGLE_USERNAME'] = 'aabdullahmian123'
os.environ['KAGGLE_KEY'] = '608ff74bb589df75702c5afaed6f46f5'

import os
import sys
import zipfile
from tqdm import tqdm

def download_casia_webface_dataset(output_dir="datasets"):
    """
    Downloads the CASIA-WebFace dataset from Kaggle
    Optimized for Google Colab environment
    """
    # Install kaggle if not already installed
    try:
        import kaggle
    except ImportError:
        print("Installing kaggle package...")
        os.system("pip install kaggle")
        import kaggle

    os.makedirs(output_dir, exist_ok=True)

    print("Setting up Kaggle authentication for Google Colab...")

    # For Google Colab, check both possible locations
    kaggle_dirs = ["/root/.kaggle", "/root/.config/kaggle"]
    kaggle_path = None

    # Find existing kaggle.json or use the config directory
    for kaggle_dir in kaggle_dirs:
        potential_path = os.path.join(kaggle_dir, "kaggle.json")
        if os.path.exists(potential_path):
            kaggle_path = potential_path
            break

    # If not found, use the config directory (Kaggle's preferred location)
    if kaggle_path is None:
        kaggle_dir = "/root/.config/kaggle"
        kaggle_path = os.path.join(kaggle_dir, "kaggle.json")

    if not os.path.exists(kaggle_path):
        print("\n" + "="*60)
        print("KAGGLE API SETUP REQUIRED")
        print("="*60)
        print("1. Go to https://www.kaggle.com/settings")
        print("2. Scroll down to 'API' section")
        print("3. Click 'Create New API Token'")
        print("4. Download the kaggle.json file")
        print("5. Upload it to this Colab session using the code below:")
        print("\n# METHOD 1 - Upload kaggle.json file:")
        print("from google.colab import files")
        print("uploaded = files.upload()")
        print("\n# METHOD 2 - Set environment variables:")
        print("import os")
        print("os.environ['KAGGLE_USERNAME'] = 'your_username'")
        print("os.environ['KAGGLE_KEY'] = 'your_api_key'")
        print("\n# Then run this script again")
        print("="*60)

        # Try to find kaggle.json in current directory (if uploaded)
        if os.path.exists("kaggle.json"):
            print("Found kaggle.json in current directory. Setting it up...")
            os.makedirs(kaggle_dir, exist_ok=True)
            import shutil
            shutil.copyfile("kaggle.json", kaggle_path)
            os.chmod(kaggle_path, 0o600)
            print("Kaggle credentials configured successfully!")
        else:
            print("\nkaggle.json not found. Please use one of the methods above.")
            return False

    try:
        print("Authenticating with Kaggle...")
        kaggle.api.authenticate()
        print("✓ Authentication successful!")

        # Download the dataset
        dataset_name = "debarghamitraroy/casia-webface"

        print(f"Downloading CASIA-WebFace dataset from {dataset_name}...")
        print("This may take several minutes depending on dataset size...")

        # Download with unzip=True to automatically extract
        kaggle.api.dataset_download_files(
            dataset_name,
            path=output_dir,
            unzip=True,
            quiet=False
        )

        print(f"✓ Dataset downloaded and extracted to {output_dir}/")

        # List contents to verify
        dataset_path = output_dir
        if os.path.exists(dataset_path):
            print(f"\nDataset contents in {dataset_path}:")
            for item in os.listdir(dataset_path)[:10]:  # Show first 10 items
                item_path = os.path.join(dataset_path, item)
                if os.path.isdir(item_path):
                    count = len(os.listdir(item_path)) if os.path.isdir(item_path) else 0
                    print(f"  📁 {item}/ ({count} items)")
                else:
                    size = os.path.getsize(item_path) / (1024*1024)  # MB
                    print(f"  📄 {item} ({size:.1f} MB)")

            total_items = len(os.listdir(dataset_path))
            if total_items > 10:
                print(f"  ... and {total_items - 10} more items")

            return dataset_path
        else:
            print("❌ Error: Dataset directory not found after download.")
            return False

    except Exception as e:
        print(f"❌ Error during download: {e}")
        print("\nTroubleshooting tips:")
        print("1. Make sure your kaggle.json file is valid")
        print("2. Check your internet connection")
        print("3. Verify the dataset exists at: https://www.kaggle.com/datasets/debarghamitraroy/casia-webface")
        return False

def main():
    """
    Main function optimized for Google Colab
    """
    print("🚀 CASIA-WebFace Dataset Downloader for Google Colab")
    print("=" * 50)

    # Use /content/ for Google Colab (persistent storage)
    output_dir = "/content/datasets"

    print(f"📁 Output directory: {output_dir}")

    result = download_casia_webface_dataset(output_dir)

    if result:
        print("\n🎉 Download completed successfully!")
        print(f"📂 Dataset location: {result}")
        print("\nYou can now use the dataset in your machine learning projects!")

        # Show how to access the data
        print("\n💡 Quick start code:")
        print("import os")
        print(f"dataset_path = '{result}'")
        print("print('Available folders:', os.listdir(dataset_path))")

    else:
        print("\n❌ Download failed. Please check the error messages above.")

if __name__ == "__main__":
    main()