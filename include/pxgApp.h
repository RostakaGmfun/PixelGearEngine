/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_APP_H
#define PXG_APP_H

class pxgEngine;

#include <string>

struct pxgVideoSettings
{
    pxgVideoSettings(int w, int h, bool fs, bool db, bool vs, std::string title, int gl_major = 3, int gl_minor = 2):
        width(w),
        height(h),
        fullscreen(fs),
        doubleBuffer(db),
        vsync(vs),
        winTitle(title),
        glMajor(gl_major),
        glMinor(gl_minor)
    {}
    pxgVideoSettings() {}
    int width;
    int height;
    bool fullscreen;
    bool doubleBuffer;
    bool vsync;
    std::string winTitle;
    int glMajor;
    int glMinor;
};


class pxgApp
{
public:
    pxgApp(pxgEngine* pxg, pxgVideoSettings vs):
        m_pxg(pxg), m_width(vs.width),
        m_height(vs.height), m_fs(vs.fullscreen),
        m_title(vs.winTitle), m_vsync(vs.vsync),
        m_dBuffer(vs.doubleBuffer),
        m_glMajor(vs.glMajor),
        m_glMinor(vs.glMinor)
    {}
    virtual bool Init() = 0;
    virtual bool Frame() = 0;
    virtual void Destroy() = 0;
    int Width() const { return m_width; }
    int Height() const { return m_height; }
    bool Fullscreen() { return m_fs; }
    std::string Title() const { return m_title; }
    bool VSync() const { return m_vsync; }
    bool DBuffer() const { return m_dBuffer; }
    pxgEngine* Engine() const { return m_pxg; }
    int GLMajor() const { return m_glMajor; }
    int GLMinor() const { return m_glMinor; }
protected:
    pxgEngine *m_pxg;
    int m_width;
    int m_height;
    bool m_fs;
    bool m_vsync;
    bool m_dBuffer;
    std::string m_title;
    int m_glMajor;
    int m_glMinor;
};

#endif //PXG_APP_H
