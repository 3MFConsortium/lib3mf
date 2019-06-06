
CBeamLattice
====================================================================================================


.. cpp:class:: Lib3MF::CBeamLattice : public CBase 

	




	.. cpp:function:: Lib3MF_double GetMinLength()

		Returns the minimal length of beams for the beamlattice.

		:returns: minimal length of beams for the beamlattice


	.. cpp:function:: void SetMinLength(const Lib3MF_double dMinLength)

		Sets the minimal length of beams for the beamlattice.

		:param dMinLength: minimal length of beams for the beamlattice 


	.. cpp:function:: void GetClipping(eBeamLatticeClipMode & eClipMode, Lib3MF_uint32 & nResourceID)

		Returns the clipping mode and the clipping-mesh for the beamlattice of this mesh.

		:param eClipMode: contains the clip mode of this mesh 
		:param nResourceID: filled with the resourceID of the clipping mesh-object or an undefined value if pClipMode is MODELBEAMLATTICECLIPMODE_NONE 


	.. cpp:function:: void SetClipping(const eBeamLatticeClipMode eClipMode, const Lib3MF_uint32 nResourceID)

		Sets the clipping mode and the clipping-mesh for the beamlattice of this mesh.

		:param eClipMode: contains the clip mode of this mesh 
		:param nResourceID: the resourceID of the clipping mesh-object. This mesh-object has to be defined before setting the Clipping. 


	.. cpp:function:: bool GetRepresentation(Lib3MF_uint32 & nResourceID)

		Returns the representation-mesh for the beamlattice of this mesh.

		:returns: flag whether the beamlattice has a representation mesh.
		:param nResourceID: filled with the resourceID of the clipping mesh-object. 


	.. cpp:function:: void SetRepresentation(const Lib3MF_uint32 nResourceID)

		Sets the representation-mesh for the beamlattice of this mesh.

		:param nResourceID: the resourceID of the representation mesh-object. This mesh-object has to be defined before setting the representation. 


	.. cpp:function:: Lib3MF_uint32 GetBeamCount()

		Returns the beam count of a mesh object.

		:returns: filled with the beam count.


	.. cpp:function:: sBeam GetBeam(const Lib3MF_uint32 nIndex)

		Returns indices, radii and capmodes of a single beam of a mesh object.

		:param nIndex: Index of the beam (0 to beamcount - 1). 
		:returns: filled with the beam indices, radii and capmodes.


	.. cpp:function:: Lib3MF_uint32 AddBeam(const sBeam & BeamInfo)

		Adds a single beam to a mesh object.

		:param BeamInfo: contains the node indices, radii and capmodes. 
		:returns: filled with the new Index of the beam.


	.. cpp:function:: void SetBeam(const Lib3MF_uint32 nIndex, const sBeam & BeamInfo)

		Sets the indices, radii and capmodes of a single beam of a mesh object.

		:param nIndex: Index of the beam (0 to beamcount - 1). 
		:param BeamInfo: filled with the beam indices, radii and capmodes. 


	.. cpp:function:: void SetBeams(const CInputVector<sBeam> & BeamInfoBuffer)

		Sets all beam indices, radii and capmodes of a mesh object.

		:param BeamInfoBuffer: contains information of a number of  beams 


	.. cpp:function:: void GetBeams(std::vector<sBeam> & BeamInfoBuffer)

		obtains all beam indices, radii and capmodes of a mesh object.

		:param BeamInfoBuffer: contains information of all beams 


	.. cpp:function:: Lib3MF_uint32 GetBeamSetCount()

		Returns the number of beamsets of a mesh object.

		:returns: filled with the beamset count.


	.. cpp:function:: PBeamSet AddBeamSet()

		Adds an empty beamset to a mesh object

		:returns: the new beamset


	.. cpp:function:: PBeamSet GetBeamSet(const Lib3MF_uint32 nIndex)

		Returns a beamset of a mesh object

		:param nIndex: index of the requested beamset (0 ... beamsetcount-1). 
		:returns: the requested beamset


.. cpp:type:: std::shared_ptr<CBeamLattice> Lib3MF::PBeamLattice

	Shared pointer to CBeamLattice to easily allow reference counting.

