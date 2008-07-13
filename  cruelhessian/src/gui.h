/*   gui.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef GUI_H
#define GUI_H


#include "CEGUI.h"
#include "RendererModules/OpenGLGUIRenderer/openglrenderer.h"
#include "CEGUIDefaultResourceProvider.h"

int read_configs();
int apply_configs();
int save_configs();

int gui_init();
int gui_start();


/*
extern CEGUI::Window* myRoot;
extern CEGUI::OpenGLRenderer *renderer;

extern CEGUI::Listbox* mMapList;
extern CEGUI::Combobox* mSchemes;

extern CEGUI::Checkbox* mIsSounds;
extern CEGUI::Checkbox* mIsMusic;
extern CEGUI::Scrollbar* mSoundsSlider;
extern CEGUI::Scrollbar* mMusicSlider;
extern CEGUI::Listbox* mMusicList;
extern CEGUI::RadioButton* mLowQuality;
extern CEGUI::RadioButton* mMediumQuality;
extern CEGUI::RadioButton* mHighQuality;

extern CEGUI::Checkbox* mIsFullscreen;
extern CEGUI::RadioButton* mDeep16;
extern CEGUI::RadioButton* mDeep32;
extern CEGUI::RadioButton* mResol640;
extern CEGUI::RadioButton* mResol800;
extern CEGUI::RadioButton* mResol1024;

extern CEGUI::PushButton* mStartGameButton;
extern CEGUI::PushButton* mDolaczGameButton;
extern CEGUI::PushButton* mExitButton;

extern CEGUI::RadioButton* mCTFButton;
extern CEGUI::RadioButton* mHTFButton;
extern CEGUI::RadioButton* mINFButton;
extern CEGUI::RadioButton* mDeathmatch;
extern CEGUI::RadioButton* mPointmatch;
extern CEGUI::RadioButton* mRambomatch;
extern CEGUI::RadioButton* mTeammatch;*/

#endif
