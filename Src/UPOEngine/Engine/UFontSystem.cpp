#if 0
// #include <ft2build.h>
// #include <freetype/freetype.h>

#include "UFontSystem.h"
#include <SpriteFont.h>

namespace UPO
{
	class CFontLoader
	{
	public:
		CFontLoader(FILE *f, CFont *font, const char *fontFile);

		virtual int Load() = 0; // Must be implemented by derived class

	protected:
		void LoadPage(int id, const char *pageFile, const char *fontFile);
		void SetFontInfo(int outlineThickness);
		void SetCommonInfo(int fontHeight, int base, int scaleW, int scaleH, int pages, bool isPacked);
		void AddChar(int id, int x, int y, int w, int h, int xoffset, int yoffset, int xadvance, int page, int chnl);
		void AddKerningPair(int first, int second, int amount);

		FILE *f;
		CFont *font;
		const char *fontFile;

		int outlineThickness;
	};

	class CFontLoaderBinaryFormat : public CFontLoader
	{
	public:
		CFontLoaderBinaryFormat(FILE *f, CFont *font, const char *fontFile);

		int Load();

		void ReadInfoBlock(int size);
		void ReadCommonBlock(int size);
		void ReadPagesBlock(int size);
		void ReadCharsBlock(int size);
		void ReadKerningPairsBlock(int size);
	};

	//=============================================================================
	// CFont
	//
	// This is the CFont class that is used to write text with bitmap fonts.
	//=============================================================================

	CFont::CFont()
	{
		fontHeight = 0;
		base = 0;
		scaleW = 0;
		scaleH = 0;
		scale = 1.0f;
		render = 0;
		hasOutline = false;
		encoding = NONE;
	}

	CFont::~CFont()
	{
		std::map<int, SCharDescr*>::iterator it = chars.begin();
		while (it != chars.end())
		{
			delete it->second;
			it++;
		}

// 		for (UINT n = 0; n < pages.size(); n++)
// 			if (pages[n])
// 				pages[n]->Release();
	}

	int CFont::Init(const char *fontFile, const char *fxFile, CDynRender *render)
	{
		this->render = render;
		this->fxFile = fxFile;

		// Load the font
		FILE *f = nullptr;
		fopen_s(&f, fontFile, "rb");

		CFontLoader* loader = new CFontLoaderBinaryFormat(f, this, fontFile);
		int r = loader->Load();
		delete loader;
		return r;
	}

	void CFont::SetTextEncoding(EFontTextEncoding encoding)
	{
		this->encoding = encoding;
	}

	// Internal
	SCharDescr *CFont::GetChar(int id)
	{
		std::map<int, SCharDescr*>::iterator it = chars.find(id);
		if (it == chars.end()) return 0;

		return it->second;
	}

	// Internal
	float CFont::AdjustForKerningPairs(int first, int second)
	{
		SCharDescr *ch = GetChar(first);
		if (ch == 0) return 0;
		for (UINT n = 0; n < ch->kerningPairs.size(); n += 2)
		{
			if (ch->kerningPairs[n] == second)
				return ch->kerningPairs[n + 1] * scale;
		}

		return 0;
	}

	float CFont::GetTextWidth(const char *text, int count)
	{
		if (count <= 0)
			count = GetTextLength(text);

		float x = 0;

		for (int n = 0; n < count; )
		{
			int charId = GetTextChar(text, n, &n);

			SCharDescr *ch = GetChar(charId);
			if (ch == 0) ch = &defChar;

			x += scale * (ch->xAdv);

			if (n < count)
				x += AdjustForKerningPairs(charId, GetTextChar(text, n));
		}

		return x;
	}

	void CFont::SetHeight(float h)
	{
		scale = h / float(fontHeight);
	}

	float CFont::GetHeight()
	{
		return scale * float(fontHeight);
	}

	float CFont::GetBottomOffset()
	{
		return scale * (base - fontHeight);
	}

	float CFont::GetTopOffset()
	{
		return scale * (base - 0);
	}

	// Internal
	// Returns the number of bytes in the string until the null char
	int CFont::GetTextLength(const char *text)
	{
// 		if (encoding == UTF16)
// 		{
// 			int textLen = 0;
// 			for (;;)
// 			{
// 				unsigned int len;
// 				int r = acUtility::DecodeUTF16(&text[textLen], &len);
// 				if (r > 0)
// 					textLen += len;
// 				else if (r < 0)
// 					textLen++;
// 				else
// 					return textLen;
// 			}
// 		}

		// Both UTF8 and standard ASCII strings can use strlen
		return (int)strlen(text);
	}

	// Internal
	int CFont::GetTextChar(const char *text, int pos, int *nextPos)
	{
// 		int ch;
// 		unsigned int len;
// 		if (encoding == UTF8)
// 		{
// 			ch = acUtility::DecodeUTF8(&text[pos], &len);
// 			if (ch == -1) len = 1;
// 		}
// 		else if (encoding == UTF16)
// 		{
// 			ch = acUtility::DecodeUTF16(&text[pos], &len);
// 			if (ch == -1) len = 2;
// 		}
// 		else
// 		{
// 			len = 1;
// 			ch = (unsigned char)text[pos];
// 		}

// 		if (nextPos) *nextPos = pos + len;
// 		return ch;
		return 0;
	}



	// Internal
	int CFont::FindTextChar(const char *text, int start, int length, int ch)
	{
		int pos = start;
		int nextPos;
		int currChar = -1;
		while (pos < length)
		{
			currChar = GetTextChar(text, pos, &nextPos);
			if (currChar == ch)
				return pos;
			pos = nextPos;
		}

		return -1;
	}

	void CFont::InternalWrite(float x, float y, float z, const char *text, int count, float spacing)
	{
		int page = -1;
		render->Begin(RENDER_QUAD_LIST);

		y += scale * float(base);

		for (int n = 0; n < count; )
		{
			int charId = GetTextChar(text, n, &n);
			SCharDescr *ch = GetChar(charId);
			if (ch == 0) ch = &defChar;

			// Map the center of the texel to the corners
			// in order to get pixel perfect mapping
			float u = (float(ch->srcX) + 0.5f) / scaleW;
			float v = (float(ch->srcY) + 0.5f) / scaleH;
			float u2 = u + float(ch->srcW) / scaleW;
			float v2 = v + float(ch->srcH) / scaleH;

			float a = scale * float(ch->xAdv);
			float w = scale * float(ch->srcW);
			float h = scale * float(ch->srcH);
			float ox = scale * float(ch->xOff);
			float oy = scale * float(ch->yOff);

			if (ch->page != page)
			{
				render->End();
				page = ch->page;
				render->GetGraphics()->SetTexture(pages[page]);
				render->Begin(RENDER_QUAD_LIST);
			}

			render->VtxColor(0xFFFFFFFF);
			render->VtxData(ch->chnl);
			render->VtxTexCoord(u, v);
			render->VtxPos(x + ox, y - oy, z);
			render->VtxTexCoord(u2, v);
			render->VtxPos(x + w + ox, y - oy, z);
			render->VtxTexCoord(u2, v2);
			render->VtxPos(x + w + ox, y - h - oy, z);
			render->VtxTexCoord(u, v2);
			render->VtxPos(x + ox, y - h - oy, z);

			x += a;
			if (charId == ' ')
				x += spacing;

			if (n < count)
				x += AdjustForKerningPairs(charId, GetTextChar(text, n));
		}

		render->End();
	}

	void CFont::Write(float x, float y, float z, const char *text, int count, unsigned int mode)
	{
		if (count <= 0)
			count = GetTextLength(text);

		if (mode == FONT_ALIGN_CENTER)
		{
			float w = GetTextWidth(text, count);
			x -= w / 2;
		}
		else if (mode == FONT_ALIGN_RIGHT)
		{
			float w = GetTextWidth(text, count);
			x -= w;
		}

		InternalWrite(x, y, z, text, count);
	}

	void CFont::WriteML(float x, float y, float z, const char *text, int count, unsigned int mode)
	{
		if (count <= 0)
			count = GetTextLength(text);

		// Get first line
		int pos = 0;
		int len = FindTextChar(text, pos, count, '\n');
		if (len == -1) len = count;
		while (pos < count)
		{
			float cx = x;
			if (mode == FONT_ALIGN_CENTER)
			{
				float w = GetTextWidth(&text[pos], len);
				cx -= w / 2;
			}
			else if (mode == FONT_ALIGN_RIGHT)
			{
				float w = GetTextWidth(&text[pos], len);
				cx -= w;
			}

			InternalWrite(cx, y, z, &text[pos], len);

			y -= scale * float(fontHeight);

			// Get next line
			pos += len;
			int ch = GetTextChar(text, pos, &pos);
			if (ch == '\n')
			{
				len = FindTextChar(text, pos, count, '\n');
				if (len == -1)
					len = count - pos;
				else
					len = len - pos;
			}
		}
	}

	void CFont::WriteBox(float x, float y, float z, float width, const char *text, int count, unsigned int mode)
	{
		if (count <= 0)
			count = GetTextLength(text);

		float currWidth = 0, wordWidth;
		int lineS = 0, lineE = 0, wordS = 0, wordE = 0;
		int wordCount = 0;

		const char *s = " ";
		float spaceWidth = GetTextWidth(s, 1);
		bool softBreak = false;

		for (; lineS < count;)
		{
			// Determine the extent of the line
			for (;;)
			{
				// Determine the number of characters in the word
				while (wordE < count &&
					GetTextChar(text, wordE) != ' ' &&
					GetTextChar(text, wordE) != '\n')
					// Advance the cursor to the next character
					GetTextChar(text, wordE, &wordE);

				// Determine the width of the word
				if (wordE > wordS)
				{
					wordCount++;
					wordWidth = GetTextWidth(&text[wordS], wordE - wordS);
				}
				else
					wordWidth = 0;

				// Does the word fit on the line? The first word is always accepted.
				if (wordCount == 1 || currWidth + (wordCount > 1 ? spaceWidth : 0) + wordWidth <= width)
				{
					// Increase the line extent to the end of the word
					lineE = wordE;
					currWidth += (wordCount > 1 ? spaceWidth : 0) + wordWidth;

					// Did we reach the end of the line?
					if (wordE == count || GetTextChar(text, wordE) == '\n')
					{
						softBreak = false;

						// Skip the newline character
						if (wordE < count)
							// Advance the cursor to the next character
							GetTextChar(text, wordE, &wordE);

						break;
					}

					// Skip the trailing space
					if (wordE < count && GetTextChar(text, wordE) == ' ')
						// Advance the cursor to the next character
						GetTextChar(text, wordE, &wordE);

					// Move to next word
					wordS = wordE;
				}
				else
				{
					softBreak = true;

					// Skip the trailing space
					if (wordE < count && GetTextChar(text, wordE) == ' ')
						// Advance the cursor to the next character
						GetTextChar(text, wordE, &wordE);

					break;
				}
			}

			// Write the line
			if (mode == FONT_ALIGN_JUSTIFY)
			{
				float spacing = 0;
				if (softBreak)
				{
					if (wordCount > 2)
						spacing = (width - currWidth) / (wordCount - 2);
					else
						spacing = (width - currWidth);
				}

				InternalWrite(x, y, z, &text[lineS], lineE - lineS, spacing);
			}
			else
			{
				float cx = x;
				if (mode == FONT_ALIGN_RIGHT)
					cx = x + width - currWidth;
				else if (mode == FONT_ALIGN_CENTER)
					cx = x + 0.5f*(width - currWidth);

				InternalWrite(cx, y, z, &text[lineS], lineE - lineS);
			}

			if (softBreak)
			{
				// Skip the trailing space
				if (lineE < count && GetTextChar(text, lineE) == ' ')
					// Advance the cursor to the next character
					GetTextChar(text, lineE, &lineE);

				// We've already counted the first word on the next line
				currWidth = wordWidth;
				wordCount = 1;
			}
			else
			{
				// Skip the line break
				if (lineE < count && GetTextChar(text, lineE) == '\n')
					// Advance the cursor to the next character
					GetTextChar(text, lineE, &lineE);

				currWidth = 0;
				wordCount = 0;
			}

			// Move to next line
			lineS = lineE;
			wordS = wordE;
			y -= scale * float(fontHeight);
		}
	}

	void CFont::PrepareEffect()
	{
		CGraphics *gfx = render->GetGraphics();

		gfx->SetEffect(fxFile.c_str());

		if (hasOutline)
			gfx->SetEffectTechnique("RenderWithOutline");
		else
			gfx->SetEffectTechnique("RenderWithoutOutline");
	}

	void CFont::PreparePixelPerfectOutput()
	{
		IDirect3DDevice9 *dev = render->GetDevice();
		CGraphics *gfx = render->GetGraphics();

		// Determine size of view port
		D3DVIEWPORT9 vp;
		dev->GetViewport(&vp);

		// Clear world matrix
		D3DXMATRIX mtx;
		D3DXMatrixIdentity(&mtx);
		gfx->SetMatrix(D3DTS_WORLD, &mtx);

		// Setup orthogonal view
		// Origin is in lower-left corner
		D3DXMatrixOrthoOffCenterLH(&mtx, 0, (float)vp.Width, 0, (float)vp.Height, vp.MinZ, vp.MaxZ);
		gfx->SetMatrix(D3DTS_VIEW, &mtx);

		D3DXMatrixIdentity(&mtx);
		gfx->SetMatrix(D3DTS_PROJECTION, &mtx);

		// Adjust the scale of the font so that the 
		// resolution of texture is the same as the screen
		scale = 1.0f;
	}

	//=============================================================================
	// CFontLoader
	//
	// This is the base class for all loader classes. This is the only class
	// that has access to and knows how to set the CFont members.
	//=============================================================================

	CFontLoader::CFontLoader(FILE *f, CFont *font, const char *fontFile)
	{
		this->f = f;
		this->font = font;
		this->fontFile = fontFile;

		outlineThickness = 0;
	}

	void CFontLoader::LoadPage(int id, const char *pageFile, const char *fontFile)
	{
// 		string str;
// 
// 		// Load the texture from the same directory as the font descriptor file
// 
// 		// Find the directory
// 		str = fontFile;
// 		for (size_t n = 0; (n = str.find('/', n)) != string::npos; ) str.replace(n, 1, "\\");
// 		size_t i = str.rfind('\\');
// 		if (i != string::npos)
// 			str = str.substr(0, i + 1);
// 		else
// 			str = "";
// 
// 		// Load the font textures
// 		str += pageFile;
// 		IDirect3DTexture9 *texture = 0;
// 		UINT mipLevels = 0; // 0 = all
// 		HRESULT hr = D3DXCreateTextureFromFileEx(font->render->GetDevice(), str.c_str(),
// 			D3DX_DEFAULT, D3DX_DEFAULT, mipLevels, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
// 			D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texture);
// 		if (FAILED(hr))
// 		{
// 			LOG(("Failed to load font page '%s' (%X)", str.c_str(), hr));
// 		}
// 		font->pages[id] = texture;
	}

	void CFontLoader::SetFontInfo(int outlineThickness)
	{
		this->outlineThickness = outlineThickness;
	}

	void CFontLoader::SetCommonInfo(int fontHeight, int base, int scaleW, int scaleH, int pages, bool isPacked)
	{
		font->fontHeight = fontHeight;
		font->base = base;
		font->scaleW = scaleW;
		font->scaleH = scaleH;
// 		font->pages.resize(pages);
// 		for (int n = 0; n < pages; n++)
// 			font->pages[n] = 0;

		if (isPacked && outlineThickness)
			font->hasOutline = true;
	}

	void CFontLoader::AddChar(int id, int x, int y, int w, int h, int xoffset, int yoffset, int xadvance, int page, int chnl)
	{
		// Convert to a 4 element vector
		// TODO: Does this depend on hardware? It probably does
		if (chnl == 1) chnl = 0x00010000;  // Blue channel
		else if (chnl == 2) chnl = 0x00000100;  // Green channel
		else if (chnl == 4) chnl = 0x00000001;  // Red channel
		else if (chnl == 8) chnl = 0x01000000;  // Alpha channel
		else chnl = 0;

		if (id >= 0)
		{
			SCharDescr *ch = new SCharDescr;
			ch->srcX = x;
			ch->srcY = y;
			ch->srcW = w;
			ch->srcH = h;
			ch->xOff = xoffset;
			ch->yOff = yoffset;
			ch->xAdv = xadvance;
			ch->page = page;
			ch->chnl = chnl;

			font->chars.insert(std::map<int, SCharDescr*>::value_type(id, ch));
		}

		if (id == -1)
		{
			font->defChar.srcX = x;
			font->defChar.srcY = y;
			font->defChar.srcW = w;
			font->defChar.srcH = h;
			font->defChar.xOff = xoffset;
			font->defChar.yOff = yoffset;
			font->defChar.xAdv = xadvance;
			font->defChar.page = page;
			font->defChar.chnl = chnl;
		}
	}

	void CFontLoader::AddKerningPair(int first, int second, int amount)
	{
		if (first >= 0 && first < 256 && font->chars[first])
		{
			font->chars[first]->kerningPairs.push_back(second);
			font->chars[first]->kerningPairs.push_back(amount);
		}
	}

	//=============================================================================
	// CFontLoaderBinaryFormat
	//
	// This class implements the logic for loading a BMFont file in binary format
	//=============================================================================
	CFontLoaderBinaryFormat::CFontLoaderBinaryFormat(FILE *f, CFont *font, const char *fontFile) : CFontLoader(f, font, fontFile)
	{
	}

	int CFontLoaderBinaryFormat::Load()
	{
		// Read and validate the tag. It should be 66, 77, 70, 2, 
		// or 'BMF' and 2 where the number is the file version.
		char magicString[4];
		fread(magicString, 4, 1, f);
		if (strncmp(magicString, "BMF\003", 4) != 0)
		{
			ULOG_MESSAGE(("Unrecognized format for '%'", fontFile));
			fclose(f);
			return -1;
		}

		// Read each block
		char blockType;
		int blockSize;
		while (fread(&blockType, 1, 1, f))
		{
			// Read the blockSize
			fread(&blockSize, 4, 1, f);

			switch (blockType)
			{
			case 1: // info
				ReadInfoBlock(blockSize);
				break;
			case 2: // common
				ReadCommonBlock(blockSize);
				break;
			case 3: // pages
				ReadPagesBlock(blockSize);
				break;
			case 4: // chars
				ReadCharsBlock(blockSize);
				break;
			case 5: // kerning pairs
				ReadKerningPairsBlock(blockSize);
				break;
			default:
				ULOG_MESSAGE("Unexpected block type (%)", blockType);
				fclose(f);
				return -1;
			}
		}

		fclose(f);

		// Success
		return 0;
	}

	void CFontLoaderBinaryFormat::ReadInfoBlock(int size)
	{
#pragma pack(push)
#pragma pack(1)
		struct infoBlock
		{
			WORD fontSize;
			BYTE reserved : 4;
			BYTE bold : 1;
			BYTE italic : 1;
			BYTE unicode : 1;
			BYTE smooth : 1;
			BYTE charSet;
			WORD stretchH;
			BYTE aa;
			BYTE paddingUp;
			BYTE paddingRight;
			BYTE paddingDown;
			BYTE paddingLeft;
			BYTE spacingHoriz;
			BYTE spacingVert;
			BYTE outline;         // Added with version 2
			char fontName[1];
		};
#pragma pack(pop)

		char *buffer = new char[size];
		fread(buffer, size, 1, f);

		// We're only interested in the outline thickness
		infoBlock *blk = (infoBlock*)buffer;
		SetFontInfo(blk->outline);

		delete[] buffer;
	}

	void CFontLoaderBinaryFormat::ReadCommonBlock(int size)
	{
#pragma pack(push)
#pragma pack(1)
		struct commonBlock
		{
			WORD lineHeight;
			WORD base;
			WORD scaleW;
			WORD scaleH;
			WORD pages;
			BYTE packed : 1;
			BYTE reserved : 7;
			BYTE alphaChnl;
			BYTE redChnl;
			BYTE greenChnl;
			BYTE blueChnl;
		};
#pragma pack(pop)

		char *buffer = new char[size];
		fread(buffer, size, 1, f);

		commonBlock *blk = (commonBlock*)buffer;

		SetCommonInfo(blk->lineHeight, blk->base, blk->scaleW, blk->scaleH, blk->pages, blk->packed ? true : false);

		delete[] buffer;
	}

	void CFontLoaderBinaryFormat::ReadPagesBlock(int size)
	{
#pragma pack(push)
#pragma pack(1)
		struct pagesBlock
		{
			char pageNames[1];
		};
#pragma pack(pop)

		char *buffer = new char[size];
		fread(buffer, size, 1, f);

		pagesBlock *blk = (pagesBlock*)buffer;

		for (int id = 0, pos = 0; pos < size; id++)
		{
			LoadPage(id, &blk->pageNames[pos], fontFile);
			pos += 1 + (int)strlen(&blk->pageNames[pos]);
		}

		delete[] buffer;
	}

	void CFontLoaderBinaryFormat::ReadCharsBlock(int size)
	{
#pragma pack(push)
#pragma pack(1)
		struct charsBlock
		{
			struct charInfo
			{
				DWORD id;
				WORD  x;
				WORD  y;
				WORD  width;
				WORD  height;
				short xoffset;
				short yoffset;
				short xadvance;
				BYTE  page;
				BYTE  chnl;
			} chars[1];
		};
#pragma pack(pop)

		char *buffer = new char[size];
		fread(buffer, size, 1, f);

		charsBlock *blk = (charsBlock*)buffer;

		for (int n = 0; int(n * sizeof(charsBlock::charInfo)) < size; n++)
		{
			AddChar(blk->chars[n].id,
				blk->chars[n].x,
				blk->chars[n].y,
				blk->chars[n].width,
				blk->chars[n].height,
				blk->chars[n].xoffset,
				blk->chars[n].yoffset,
				blk->chars[n].xadvance,
				blk->chars[n].page,
				blk->chars[n].chnl);
		}

		delete[] buffer;
	}

	void CFontLoaderBinaryFormat::ReadKerningPairsBlock(int size)
	{
#pragma pack(push)
#pragma pack(1)
		struct kerningPairsBlock
		{
			struct kerningPair
			{
				DWORD first;
				DWORD second;
				short amount;
			} kerningPairs[1];
		};
#pragma pack(pop)

		char *buffer = new char[size];
		fread(buffer, size, 1, f);

		kerningPairsBlock *blk = (kerningPairsBlock*)buffer;

		for (int n = 0; int(n * sizeof(kerningPairsBlock::kerningPair)) < size; n++)
		{
			AddKerningPair(blk->kerningPairs[n].first,
				blk->kerningPairs[n].second,
				blk->kerningPairs[n].amount);
		}

		delete[] buffer;
	}

};
#endif // 0
