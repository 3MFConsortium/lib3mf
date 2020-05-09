package com.aibuild.lib3mf4j;

import com.aibuild.lib3mf4j.*;

import static org.junit.Assert.*;
import org.junit.Test;

public class Lib3MFWrapperTest {

    public static String libpath = "3mf";

    @Test
    public void correctlyReadsVersion() {
        final Lib3MFWrapper wrapper = new Lib3MFWrapper(libpath);
        int major = -1;
        int minor = -1;
        int micro = -1;

        try {
            Lib3MFWrapper.GetLibraryVersionResult version = wrapper.getLibraryVersion();
            major = version.Major;
            minor = version.Minor;
            micro = version.Micro;
        }
        catch (Lib3MFException e) {
            System.out.println(e);
        }
        assertEquals(major, 2);
        assertEquals(minor, 0);
        assertEquals(micro, 0);
    }
}