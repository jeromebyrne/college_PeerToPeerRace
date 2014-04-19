#include "precompiled.h"
#include "texture.h"
 Texture * Texture:: m_instance = 0;

Texture * Texture ::GetInstance()
{
	if(m_instance == 0)
	{
		m_instance = new Texture();
	}
	return m_instance;
}

void Texture::Initialise(HWND &hWndMain)
{
	d_hWndMain = hWndMain;
	glGenTextures(9, texture);// generate texture object names
}

Texture :: Texture():d_numTextures(0)
{
    

}
// LoadBitmapFile
// desc: Returns a pointer to the bitmap image of the bitmap specified
//       by filename. Also returns the bitmap header information.
//		 No support for 8-bit bitmaps.
unsigned char * Texture :: LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
        	FILE *pFile;							// the file pointer
        	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
        	unsigned char		*bitmapImage;			// bitmap image data
        	unsigned int		imageIdx = 0;		// image index counter
        	unsigned char		tempRGB;				// swap variable
        
        	// open filename in "read binary" mode
        	pFile = fopen(filename, "rb");
        	if (pFile == NULL)
        		return NULL;
        
        	// read the bitmap file header
        	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
        	
        	// verify that this is a bitmap by checking for the universal bitmap id
        	if (bitmapFileHeader.bfType != BITMAP_ID)
        	{
        		fclose(pFile);
        		return NULL;
        	}
        
        	// read the bitmap information header
        	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
        
        	// move file pointer to beginning of bitmap data
        	fseek(pFile, bitmapFileHeader.bfOffBits, SEEK_SET);
        
        	// allocate enough memory for the bitmap image data
        	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
        
        	// verify memory allocation
        	if (!bitmapImage)
        	{
        		delete [] bitmapImage;
        		fclose(pFile);
        		return NULL;
        	}
        
        	// read in the bitmap image data
        	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, pFile);
        	
        	// make sure bitmap image data was read
        	if (bitmapImage == NULL)
        	{
        		fclose(pFile);	
        		return NULL;
        	}
        
        	// swap the R and B values to get RGB since the bitmap color format is in BGR
        	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
        	{
        		tempRGB = bitmapImage[imageIdx];
        		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        		bitmapImage[imageIdx + 2] = tempRGB;
        	}
        
        	// close the file and return the bitmap image data
        	fclose(pFile);
        
        	return bitmapImage;
}
unsigned int Texture :: generateTexture(char * string)
{
                 bitmapData=NULL;
                 bitmapData = LoadBitmapFile(string, &bitmapInfoHeader);
                 
                 if(!bitmapData)
				 {
					 MessageBox(d_hWndMain,(LPCSTR)"Bitmap reading error",(LPCSTR)"Error",MB_OK);
				 }

                     glBindTexture(GL_TEXTURE_2D, texture[d_numTextures]);       // enable our texture object to be 2D
            	     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            	     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     
                     // generate the texture image
                	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
				 bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
	//CheckGL();	
	delete [] bitmapData;
	unsigned int textures = d_numTextures;
	d_numTextures++;
	return texture[textures];	
}
