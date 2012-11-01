#include <irrlicht.h>
#include "driverChoice.h"
#include "tileEngine.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{

    tile_engine::tileMap carcassone_board;

    video::E_DRIVER_TYPE driverType=video::EDT_OPENGL;//driverChoiceConsole();
    if (driverType==video::EDT_COUNT)
        return 1;

    // create device
    IrrlichtDevice *device = createDevice(driverType,
            core::dimension2d<u32>(800, 600));
    //core::dimension2d<u32>(512, 384));

    if (device == 0)
        return 1; // could not create selected driver.

    device->setWindowCaption(L"Irrlicht Engine - 2D Graphics Demo");
    video::IVideoDriver* driver = device->getVideoDriver();

    video::ITexture* images = driver->getTexture("../../media/carc_tiles.png");
    driver->makeColorKeyTexture(images, core::position2d<s32>(0,0));

    gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
    gui::IGUIFont* font2 =
        device->getGUIEnvironment()->getFont("../../media/fonthaettenschweiler.bmp");

    core::rect<s32> imp1(349,15,385,78);
    core::rect<s32> imp2(387,15,423,78);

    /*
       Prepare a nicely filtering 2d render mode for special cases.
       */
    driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
    driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

    while(device->run() && driver)
    {
        if (device->isWindowActive())
        {
            u32 time = device->getTimer()->getTime();

            if(time/500 %2)
                carcassone_board.generateNextTile();

            driver->beginScene(true, true, video::SColor(255,120,102,136));

            driver->enableMaterial2D();

            // loop thru board, draw each tile
            //driver->draw2DImage(images, core::rect<s32>(10,10,90,90),
            //tile_engine::tileTexture::getSourceRectangle(images, 3));
            for (int i = 0; i < carcassone_board.rows[0].size(); i++) 
            {
                auto tm = carcassone_board.rows[0][i];
                driver->draw2DImage(images, core::rect<s32>(i*80,0,80*(i+1),80),
                        tile_engine::tileTexture::getSourceRectangle(images, tm.tileID));
            }


            driver->enableMaterial2D(false);

            core::position2d<s32> m = device->getCursorControl()->getPosition();
            driver->draw2DRectangle(video::SColor(100,255,255,255),
                    core::rect<s32>(m.X-20, m.Y-20, m.X+20, m.Y+20));

            driver->endScene();
        }
    }

    device->drop();

    return 0;
}

/*
   That's all. I hope it was not too difficult.
 **/
