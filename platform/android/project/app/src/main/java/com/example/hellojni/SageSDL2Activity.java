package com.example.hellojni;

import org.libsdl.app.SDLActivity;

public class SageSDL2Activity extends SDLActivity{
    private final String LIB_NAME = "sageExample";

	@Override
	protected String[] getLibraries(){
		return new String[]{LIB_NAME};
	}
}
