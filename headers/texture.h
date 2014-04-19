#ifndef TEXTURE_H
#define TEXTURE_H
#define BITMAP_ID 0x4D42		// the universal bitmap ID
class Texture{
      private:
              unsigned int d_numTextures;//the current number of textures generated
              BITMAPINFOHEADER	bitmapInfoHeader;	// bitmap info header
              unsigned char* bitmapData;			// the texture data
              unsigned int	texture[10];//textures
              HWND d_hWndMain;

			  unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);//loads a bitmap

			  static Texture * m_instance;
      public:
             Texture();
             unsigned int generateTexture(char * string);//generates a texture and gives it to whatever called it
			 static Texture * GetInstance();
			 void Initialise(HWND &hWndMain);
             
      };
#endif
