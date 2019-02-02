package com.frequem.sage.test;

public class MainActivity extends org.libsdl.app.SDLActivity{
	@Override
    protected String[] getLibraries() {
        return new String[]{
				"SDL2main",
                "SDL2",
                "SDL2_ttf",
                "freetype",
                "SOIL",
                "sage",
                "sageExample"
        };
    }
}
