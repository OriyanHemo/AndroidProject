package com.example.nso_project;
//Utils.java class
public class Utils {
    static {
        System.loadLibrary("dynamicloader");
    }
    //This function pathching a lib as native function
    public native void patchLib(String libPath);
}
