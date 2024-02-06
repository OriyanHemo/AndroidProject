package com.example.nso_project;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.content.Context;
import android.content.res.Resources;


import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {
  
    final int LIB_HELLO_LIBRARY = 1;
    final int LIB_GADGET_LIBRARY = 2;
    
    static {
        System.loadLibrary("dynamicloader");
    }
    /*
    *This function run on create
    *
    *Input: Bundle savedInstanceState
    *Output: void
    */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button loadLibButton = findViewById(R.id.loadLibButton);//create button for loadlib native function
        loadLibButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                loadAndInitLib("libhello.so");
            }
        });

        Button unloadLibButton = findViewById(R.id.unloadLibButton);//create button for unloadlib native function
        unloadLibButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                unloadAndInitLib("libhello.so");
            }
        });

        Button patchLibButton = findViewById(R.id.patchLibButton);//create button for patchlib function
        patchLibButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                patchAndInitLib("libgadget.so");
            }
        });
    }
    /*
    *This function load and init lib
    *
    *Input: String libName
    *Output: void
    */
    private void loadAndInitLib(String libName) {
        copyLibToInternalStorage(LIB_HELLO_LIBRARY, libName);
        loadLib(libName);
    }
    
    /*
    *This function unload and init lib
    *
    *Input: String libName
    *Output: void
    */
    private void unloadAndInitLib(String libName) {
        copyLibToInternalStorage(LIB_HELLO_LIBRARY, libName);
        unloadLib(libName);
    }
    
    /*
    *This function patch and init lib
    *
    *Input: String libName
    *Output: void
    */
    private void patchAndInitLib(String libName) {

        Utils utils = new Utils();
        copyLibToInternalStorage(LIB_GADGET_LIBRARY, libName);
        utils.patchLib(libName);
    }

    /*
    *This function copy lib to internal storage
    *
    *Input: int choice
    *Output: void
    */
    private void copyLibToInternalStorage(int choice, String libName) {
        try {
            Context con = getApplicationContext();
            Resources res = con.getResources();
            int libResId = 0;
            
            if(LIB_HELLO_LIBRARY == choice){
              libResId = R.raw.libhello;
            }
            else if(LIB_GADGET_LIBRARY == choice){
              libResId = R.raw.libgadget;
            }
            File internalDirectory = getDir("lib", Context.MODE_PRIVATE);
            File libFile = new File(internalDirectory, libName);
            try (InputStream inputStream = res.openRawResource(libResId);
                 FileOutputStream outputStream = new FileOutputStream(libFile)) {
                byte[] buffer = new byte[4096];
                int bytesRead;
                while ((bytesRead = inputStream.read(buffer)) != -1) {
                    outputStream.write(buffer, 0, bytesRead);
                }
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    //This function load a lib as native function
    private native void loadLib(String libPath);
    //This function unload a lib as native function
    private native void unloadLib(String libPath);
}
