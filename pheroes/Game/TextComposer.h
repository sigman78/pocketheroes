/*
 * This file is a part of Pocket Heroes Game project
 * 	http://www.pocketheroes.net
 *	https://code.google.com/p/pocketheroes/
 *
 * Copyright 2004-2010 by Robert Tarasov and Anton Stuk (iO UPG)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ 

#ifndef _HMM_GAME_TEXT_COMPOSER_H_
#define _HMM_GAME_TEXT_COMPOSER_H_

class iTextComposer
{
public:
	enum FontSize {
		FS_SMALL = 0,
		FS_MEDIUM,
		FS_LARGE,
		FS_COUNT
	};	

	struct FontConfig {
		FontConfig(FontSize _fontSize, const iDibFont::ComposeProps& _cmpProps) 
		: fontSize(_fontSize), cmpProps(_cmpProps){}
		
		FontConfig(const FontConfig& other) 
		: fontSize(other.fontSize), cmpProps(other.cmpProps){}

		FontSize					fontSize;
		iDibFont::ComposeProps		cmpProps;
	};

public:
	iTextComposer();
	bool Init();
	void SetFontSize(FontSize fs);
	iStringT CropString(const iStringT& text, FontConfig &fc, uint32 width, bool bDot=true);
	iSize GetTextSize(const iStringT& text, FontConfig cfg);
	iSize GetTextBoxSize(const iStringT& text, uint32 width, FontConfig fc);
	void TextOut(const FontConfig &fc, iDib& dib, const iPoint& pos, const iStringT& text, const iRect& orc=cInvalidRect, Alignment al = AlignTopLeft, const iPoint& offset = iPoint());
	void TextOut(iDib& dib, const iPoint& pos, const iStringT& text, const iRect& orc=cInvalidRect, Alignment al=AlignTopLeft, const iPoint& offset = iPoint());
	sint32 TextBoxOut(const FontConfig &fc, iDib& dib, const iStringT& text, const iRect& orc);
	sint32 TextBoxOut(iDib& dib, const iStringT& text, const iRect& orc);
	
private:
	FontConfig	m_fontConfig;
	iDibFont	m_Fonts[FS_COUNT];
};

#endif //_HMM_GAME_TEXT_COMPOSER_H_

