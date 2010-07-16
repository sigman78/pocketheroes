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

#ifndef _GXLIB_GENERIC_VIEW_H_
#define _GXLIB_GENERIC_VIEW_H_

enum VIEWCLSID {
	GENERIC_VIEWPORT = 0,
	MODAL_DIALOG,
	PUSH_BUTTON,
	TABBED_SWITCH,
	SCROLL_BAR,
	LIST_BOX
};

//////////////////////////////////////////////////////////////////////////
enum CTRL_EVT_ID {
	CEI_BUTTONDOWN = 0,
	CEI_BUTTONUP,
	CEI_TRACKING,
	CEI_CLICK
};

//////////////////////////////////////////////////////////////////////////
class iViewMgr;
class iView
{
public:
	enum ViewState {
		Visible	= 0x1,
		Enabled = 0x2
	};

public:
	iView(iViewMgr* pViewMgr, const iRect& rect, uint32 clsId, uint32 uid, uint32 state);
	virtual ~iView();

	bool AddChild(iView* pChild);
	bool RemoveChild(iView* pChild);
	iView* GetChildByPos(const iPoint& pos) const;
	iView* GetChildById(uint32 uid) const;

	// Message process
	void Compose(iRect& rect);
	bool MouseDown(const iPoint& pos);
	bool MouseUp(const iPoint& pos);
	bool MouseTrack(const iPoint& pos);

	// Overrides
	virtual void OnTimer(uint32 tid) { };
	virtual void OnCompose() { };
	virtual void OnRectChanged(const iRect& rc) {}
	virtual void OnMouseDown(const iPoint& pos) {}
	virtual void OnMouseUp(const iPoint& pos) {}
	virtual void OnMouseClick(const iPoint& pos) {}
	virtual void OnMouseTrack(const iPoint& pos) {}
	
	// View metrics
	void SetSize(const iSize& nsiz);
	void SetPos(const iPoint& npos);
	void SetRect(const iRect& rc);

	// inlines
	inline iSize GetSize() const { return m_Rect.size(); }
	inline iPoint GetPos() const { return m_Rect.point(); }
	inline iRect GetRect() const { return m_Rect; }
	inline iPoint GetScrPos() const { iPoint res(m_Rect.point()); if (m_pParent) res+=m_pParent->GetScrPos();  return res; }
	inline iRect GetScrRect() const { iRect res(m_Rect); if(m_pParent) res += m_pParent->GetScrPos(); return res; }
	inline iView* GetParent() const { return m_pParent; }
	inline bool IsEnabled() const { return m_bEnabled && (!m_pParent || (m_pParent && m_pParent->IsEnabled())); }
	inline void SetEnabled(bool bEnabled=true) { m_bEnabled = bEnabled; Invalidate(); }
	inline bool IsVisible() const { return m_bVisible; }
	inline void SetVisible(bool bVisible = true) { m_bVisible = bVisible; Invalidate(); }
	inline uint32 GetClassId() const { return m_clsId; }
	inline uint32 GetUID() const { return m_UID; }
	inline bool NeedRedraw() const { return m_bNeedRedraw; }
	virtual void Invalidate();

private:
	bool CreateView(iViewMgr* pViewMgr, const iRect& rect, uint32 clsId, uint32 uid, uint32 state);
	void Cleanup();

protected:
	typedef iSimpleArray<iView*> iViewList;

	uint32				m_UID;
	uint32				m_clsId;
	iRect				m_Rect;

	bool				m_bEnabled;
	bool				m_bVisible;
	bool				m_bTracking;
	bool				m_bNeedRedraw;
	
	iViewMgr*			m_pMgr;
	iView*				m_pParent;
	iViewList			m_Childs;
};

#endif // _GXLIB_GENERIC_VIEW_H_

