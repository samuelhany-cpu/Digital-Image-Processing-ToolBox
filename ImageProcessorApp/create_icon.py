from PIL import Image, ImageDraw, ImageFont
import os

def create_app_icon():
    """Create a professional icon for the Mexo Toolbox Image Processing Application"""
    
    # Create multiple sizes for Windows .ico format
    sizes = [256, 128, 64, 48, 32, 16]
    images = []
    
    for size in sizes:
        # Create a new image with dark blue background
        img = Image.new('RGBA', (size, size), (10, 14, 39, 255))  # #0a0e27
        draw = ImageDraw.Draw(img)
        
        # Calculate proportional dimensions
        border_width = max(2, size // 32)
        padding = size // 8
        
        # Draw outer border (cyan accent)
        draw.rectangle(
            [border_width, border_width, size - border_width, size - border_width],
            outline=(0, 212, 255, 255),  # #00d4ff cyan
            width=border_width
        )
        
        # Draw inner grid pattern (representing image pixels/processing)
        grid_padding = size // 6
        grid_size = size - 2 * grid_padding
        grid_divisions = 3
        cell_size = grid_size // grid_divisions
        
        # Draw gradient grid cells
        colors = [
            (0, 212, 255, 200),    # Cyan
            (31, 166, 90, 200),    # Green
            (255, 0, 110, 200),    # Pink
            (57, 255, 20, 200),    # Lime
        ]
        
        for i in range(grid_divisions):
            for j in range(grid_divisions):
                x1 = grid_padding + i * cell_size + border_width
                y1 = grid_padding + j * cell_size + border_width
                x2 = x1 + cell_size - border_width * 2
                y2 = y1 + cell_size - border_width * 2
                
                color = colors[(i + j) % len(colors)]
                draw.rectangle([x1, y1, x2, y2], fill=color)
        
        # Draw central "M" for Mexo (if size is large enough)
        if size >= 64:
            try:
                # Try to use a nice font, fall back to default if not available
                font_size = size // 3
                try:
                    font = ImageFont.truetype("arial.ttf", font_size)
                except:
                    font = ImageFont.load_default()
                
                # Draw "M" in the center
                text = "S"
                bbox = draw.textbbox((0, 0), text, font=font)
                text_width = bbox[2] - bbox[0]
                text_height = bbox[3] - bbox[1]
                
                text_x = (size - text_width) // 2
                text_y = (size - text_height) // 2 - size // 20
                
                # Draw shadow
                draw.text((text_x + 2, text_y + 2), text, fill=(0, 0, 0, 180), font=font)
                # Draw main text
                draw.text((text_x, text_y), text, fill=(255, 255, 255, 255), font=font)
            except:
                pass
        
        images.append(img)
    
    # Save as .ico file
    icon_path = 'resources/icons/mexo_toolbox_logo.ico'
    os.makedirs(os.path.dirname(icon_path), exist_ok=True)
    
    # Save multi-resolution .ico
    images[0].save(
        icon_path,
        format='ICO',
        sizes=[(img.width, img.height) for img in images],
        append_images=images[1:]
    )
    
    # Also save a PNG version for documentation
    images[0].save('resources/icons/mexo_toolbox_logo.png', format='PNG')
    
    print(f"✓ Icon created successfully!")
    print(f"  - ICO file: {icon_path}")
    print(f"  - PNG file: resources/icons/mexo_toolbox_logo.png")
    print(f"  - Sizes: {', '.join(f'{s}x{s}' for s in sizes)}")

if __name__ == '__main__':
    create_app_icon()
