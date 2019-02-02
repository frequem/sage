package com.frequem.sage.test;

public class Test{
	static{
		System.loadLibrary("native-lib");
	}
	
	public native String stringFromJNI(String name);
}
