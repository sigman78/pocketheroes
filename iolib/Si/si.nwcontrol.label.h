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

/** @file
 *	@brief NonWindowed text label (static) control class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_NWCONTROL_LABEL_H_
#define _IOLIB_SILICON_NWCONTROL_LABEL_H_

/// NonWindowed text label (static) control class
class iNWLabel : public iNWControl
{
public:
	/// Creates and initializes text label control
	bool CreateNWLabel(iNWElementContainerI *parent, const iRect &rect, const iStringT &text = iStringT(_T("Text Label")));

	virtual void DrawElement(iMemoryDC &ddc) const;
	virtual HCURSOR GetCursor(const iPoint &pos) const;

	/// Sets label font, text color, background color and text alignment
	void SetLabelProps(iFont &fnt, const iColor &txt_clr, const iColor &bck_clr, iAlignment algn);

	/// Sets label text
	void SetLabelText(const iStringT &ntext);
	/// Sets label font
	void SetLabelFont(iFont &fnt);
	/// Sets label text color
	void SetLabelTextColor(const iColor &clr);
	/// Sets label background color
	void SetLabelBackColor(const iColor &clr);
	/// Sets label text alignment
	void SetLabelAlignment(iAlignment algn);


private:
	/// Draws label
	void DrawLabel();

	iDibText	m_DibText;
	iStringT	m_Text;
	iDib		m_Dib;
	iColor		m_BackColor;
};

#endif //_IOLIB_SILICON_NWCONTROL_LABEL_H_
/** @} */ // iolib_si
