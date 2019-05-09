package com.example.sage.new_sage_project;

import org.libsdl.app.SDLActivity;

public class SageSDL2Activity extends SDLActivity{
    private final String LIB_NAME = "new_sage_project";

	@Override
	protected String[] getLibraries(){
		return new String[]{LIB_NAME};
	}
}
