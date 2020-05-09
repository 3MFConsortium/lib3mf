package com.aibuild.lib3mf4j;

import com.aibuild.lib3mf4j.*;

import static org.junit.Assert.*;
import org.junit.Test;

public class Lib3MFWrapperTest {

    final Lib3MFWrapper wrapper = new Lib3MFWrapper("3mf");

    @Test
    public void correctlyReadsVersion() {
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

    @Test
    public void correctlyWritesAndReadsBeamLatticeFile() {
        try {
            final Model model = wrapper.createModel();

            final MeshObject meshObject = model.addMeshObject();
            meshObject.addVertex(new Position(12, 12, 0));
            meshObject.addVertex(new Position(3, 5, 0));
            meshObject.addVertex(new Position(21, 3, 0));
            meshObject.addVertex(new Position(3, 7, 2));
            meshObject.addVertex(new Position(11, 3, 4));

            final BeamLattice beamLattice = meshObject.beamLattice();
            beamLattice.addBeam(new Beam(0, 1, 2.0f, 1.4f));
            beamLattice.addBeam(new Beam(1, 2, 2.0f, 1.4f));
            beamLattice.addBeam(new Beam(2, 3, 2.0f, 1.4f));
            beamLattice.addBeam(new Beam(4, 2, 1.0f, 0.4f));

            final String filePath = "target/beamLatticeTest.3mf";

            final Writer writer = model.queryWriter("3mf");
            writer.writeToFile(filePath);

            final Model importedModel = wrapper.createModel();
            final Reader reader = importedModel.queryReader("3mf");
            reader.readFromFile(filePath);

            final MeshObject importedMesh = importedModel.getMeshObjectByID(1);

            assertEquals(importedMesh.getVertexCount(), 5);
            assertEquals(importedMesh.getVertex(0).Coordinates[0], 12, 0.001f);
            assertEquals(importedMesh.getVertex(0).Coordinates[1], 12, 0.001f);
            assertEquals(importedMesh.getVertex(0).Coordinates[2], 0, 0.001f);
            assertEquals(importedMesh.getVertex(4).Coordinates[0], 11, 0.001f);
            assertEquals(importedMesh.getVertex(4).Coordinates[1], 3, 0.001f);
            assertEquals(importedMesh.getVertex(4).Coordinates[2], 4, 0.001f);

            assertEquals(importedMesh.beamLattice().getBeamCount(), 4);
            assertEquals(importedMesh.beamLattice().getBeam(0).Indices[0], 0);
            assertEquals(importedMesh.beamLattice().getBeam(0).Indices[1], 1);
            assertEquals(importedMesh.beamLattice().getBeam(0).Radii[0],
                2.0f, 0.001f);
            assertEquals(importedMesh.beamLattice().getBeam(0).Radii[1],
                1.4f, 0.001f);
            assertEquals(importedMesh.beamLattice().getBeam(3).Indices[0], 4);
            assertEquals(importedMesh.beamLattice().getBeam(3).Indices[1], 2);
            assertEquals(importedMesh.beamLattice().getBeam(3).Radii[0],
                1.0f, 0.001f);
            assertEquals(importedMesh.beamLattice().getBeam(3).Radii[1],
                0.4f, 0.001f);
        }
        catch (Lib3MFException e) {
            System.out.println(e);
        }
    }
}