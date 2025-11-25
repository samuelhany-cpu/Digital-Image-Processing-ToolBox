import os
import glob
import re

def remove_emojis_from_file(file_path):
    """Remove all emojis and special Unicode characters from a file."""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original_content = content
        
        # Define comprehensive emoji and symbol replacements
        replacements = [
            # Window titles
            ('?? Image Transformation', 'Image Transformation'),
            ('?? Image Translation Settings', 'Image Translation Settings'),
            ('?? Image Rotation Settings', 'Image Rotation Settings'),
            ('?? Image Zoom Settings', 'Image Zoom Settings'),
            
            # Direction indicators
            ('? Right  |  ? Left', 'Right  |  Left'),
            ('? Down  |  ? Up', 'Down  |  Up'),
            ('?? Clockwise  |  ?? Counter-clockwise', 'Clockwise  |  Counter-clockwise'),
            ('?? Zoom In  |  ?? Zoom Out', 'Zoom In  |  Zoom Out'),
            
            # Group boxes
            ('? X Translation (Horizontal)', 'X Translation (Horizontal)'),
            ('? Y Translation (Vertical)', 'Y Translation (Vertical)'),
            ('?? Rotation Angle', 'Rotation Angle'),
            ('?? Zoom Factor', 'Zoom Factor'),
            
            # Button labels
            ('? Cancel', 'Cancel'),
            ('? Apply Translation', 'Apply Translation'),
            ('? Apply Rotation', 'Apply Rotation'),
            ('? Apply Zoom', 'Apply Zoom'),
            
            # Status messages
            ('?? Loading image...', 'Loading image...'),
            ('?? Saving image...', 'Saving image...'),
            ('? Image saved successfully', 'Image saved successfully'),
            ('? Failed to save image', 'Failed to save image'),
            ('?? Image reset to original', 'Image reset to original'),
            ('? Failed to load image', 'Failed to load image'),
            
            # Transformation messages
            ('? Image translated successfully', 'Image translated successfully'),
            ('? Image rotated successfully', 'Image rotated successfully'),
            ('? Image skewed successfully', 'Image skewed successfully'),
            ('? Image zoomed successfully', 'Image zoomed successfully'),
            ('? Image flipped horizontally', 'Image flipped horizontally'),
            ('? Image flipped vertically', 'Image flipped vertically'),
            ('? Image flipped both ways', 'Image flipped both ways'),
            
            # Histogram messages
            ('? Histogram equalization applied', 'Histogram equalization applied'),
            ("? Otsu's thresholding applied", "Otsu's thresholding applied"),
            
            # Processing messages
            ('? Converted to grayscale', 'Converted to grayscale'),
            ('? Binary threshold applied', 'Binary threshold applied'),
            ('? Gaussian blur applied', 'Gaussian blur applied'),
            ('? Edge detection applied', 'Edge detection applied'),
            ('? Colors inverted', 'Colors inverted'),
            
            # Filter messages
            ('📊 Applying traditional filter...', 'Applying traditional filter...'),
            ('✅ Traditional filter applied successfully', 'Traditional filter applied successfully'),
            ('🔺 Applying pyramidal filter...', 'Applying pyramidal filter...'),
            ('✅ Pyramidal filter applied successfully', 'Pyramidal filter applied successfully'),
            ('⭕ Applying circular filter...', 'Applying circular filter...'),
            ('✅ Circular filter applied successfully', 'Circular filter applied successfully'),
            ('🔻 Applying cone filter...', 'Applying cone filter...'),
            
            # Dialog titles
            ('?? Image Information', 'Image Information'),
            ('?? Detailed Image Information', 'Detailed Image Information'),
            ('?? Pixel Information', 'Pixel Information'),
            ('?? Get Pixel Value', 'Get Pixel Value'),
            ('?? Image Statistics', 'Image Statistics'),
            ('?? Image Histogram', 'Image Histogram'),
            ('?? Pixel Value Distribution', 'Pixel Value Distribution'),
            ('? Close', 'Close'),
            
            # Filter dialog titles
            ('📊 Traditional Filter', 'Traditional Filter'),
            ('🔺 Pyramidal Filter', 'Pyramidal Filter'),
            ('⭕ Circular Filter', 'Circular Filter'),
            
            # Unicode symbols
            ('×', 'x'),
            ('�', 'degrees'),
        ]
        
        # Apply all replacements
        for old, new in replacements:
            content = content.replace(old, new)
        
        # Remove any remaining emoji characters using regex
        # This regex matches most emoji ranges
        emoji_pattern = re.compile(
            "["
            "\U0001F600-\U0001F64F"  # emoticons
            "\U0001F300-\U0001F5FF"  # symbols & pictographs
            "\U0001F680-\U0001F6FF"  # transport & map symbols
            "\U0001F1E0-\U0001F1FF"  # flags (iOS)
            "\U00002702-\U000027B0"
            "\U000024C2-\U0001F251"
            "\U0001F900-\U0001F9FF"  # Supplemental Symbols and Pictographs
            "\U00002600-\U000026FF"  # Miscellaneous Symbols
            "\U00002700-\U000027BF"  # Dingbats
            "]+", 
            flags=re.UNICODE
        )
        content = emoji_pattern.sub('', content)
        
        # Only write if content changed
        if content != original_content:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            return True
        return False
        
    except Exception as e:
        print(f"  Error processing {file_path}: {e}")
        return False

def main():
    """Scan all C++ source files and remove emojis."""
    print("=== Emoji Removal Tool ===\n")
    
    # Define patterns for files to process
    file_patterns = [
        'src/*.cpp',
        'src/*.h',
        'include/*.h',
        '*.cpp',
        '*.h'
    ]
    
    files_processed = 0
    files_modified = 0
    
    # Process all matching files
    for pattern in file_patterns:
        for file_path in glob.glob(pattern, recursive=False):
            if os.path.isfile(file_path):
                files_processed += 1
                print(f"Processing: {file_path}...")
                if remove_emojis_from_file(file_path):
                    files_modified += 1
                    print(f"  ✓ Modified")
                else:
                    print(f"  - No changes needed")
    
    print(f"\n=== Summary ===")
    print(f"Files processed: {files_processed}")
    print(f"Files modified: {files_modified}")
    print(f"\nEmoji removal complete!")

if __name__ == '__main__':
    main()
