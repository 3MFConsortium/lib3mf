
CBeamLattice
====================================================================================================


.. cpp:class:: Lib3MF::CBeamLattice : public CBase 

	




	.. cpp:function:: Lib3MF_double GetMinLength()

		Returns the minimal length of beams for the beamlattice.

		:returns: minimal length of beams for the beamlattice


	.. cpp:function:: void SetMinLength(const Lib3MF_double dMinLength)

		Sets the minimal length of beams for the beamlattice.

		:param dMinLength: minimal length of beams for the beamlattice 


	.. cpp:function:: void GetClipping(eBeamLatticeClipMode & eClipMode, Lib3MF_uint32 & nUniqueResourceID)

		Returns the clipping mode and the clipping-mesh for the beamlattice of this mesh.

		:param eClipMode: contains the clip mode of this mesh 
		:param nUniqueResourceID: filled with the UniqueResourceID of the clipping mesh-object or an undefined value if pClipMode is MODELBEAMLATTICECLIPMODE_NONE 


	.. cpp:function:: void SetClipping(const eBeamLatticeClipMode eClipMode, const Lib3MF_uint32 nUniqueResourceID)

		Sets the clipping mode and the clipping-mesh for the beamlattice of this mesh.

		:param eClipMode: contains the clip mode of this mesh 
		:param nUniqueResourceID: the UniqueResourceID of the clipping mesh-object. This mesh-object has to be defined before setting the Clipping. 


	.. cpp:function:: bool GetRepresentation(Lib3MF_uint32 & nUniqueResourceID)

		Returns the representation-mesh for the beamlattice of this mesh.

		:returns: flag whether the beamlattice has a representation mesh.
		:param nUniqueResourceID: filled with the UniqueResourceID of the clipping mesh-object. 


	.. cpp:function:: void SetRepresentation(const Lib3MF_uint32 nUniqueResourceID)

		Sets the representation-mesh for the beamlattice of this mesh.

		:param nUniqueResourceID: the UniqueResourceID of the representation mesh-object. This mesh-object has to be defined before setting the representation. 
    
    
  .. cpp:function:: void GetBallOptions(eBeamLatticeBallMode & eBallMode, Lib3MF_double & dBallRadius)

		Returns the ball mode and the default ball radius for the beamlattice of this mesh.Returns the ball mode and the default ball radius for the beamlattice of this mesh.

		:param eBallMode: contains the ball mode of this mesh 
		:param dBallRadius: default ball radius of balls for the beamlattice 


	.. cpp:function:: void SetBallOptions(const eBeamLatticeBallMode eBallMode, const Lib3MF_double dBallRadius)

		Sets the ball mode and thedefault ball radius for the beamlattice.

		:param eBallMode: contains the ball mode of this mesh 
		:param dBallRadius: default ball radius of balls for the beamlattice 

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


    .. cpp:function:: Lib3MF_uint32 GetBallCount()

		Returns the ball count of a mesh object.

		:returns: filled with the ball count.


	.. cpp:function:: sBall GetBall(const Lib3MF_uint32 nIndex)

		Returns index and radius of a single ball of a mesh object.

		:param nIndex: Index of the ball (0 to ballcount - 1). 
		:returns: filled with the ball node index and radius.


	.. cpp:function:: Lib3MF_uint32 AddBall(const sBall & BallInfo)

		Adds a single ball to a mesh object.

		:param BallInfo: contains the node index and radius. 
		:returns: filled with the new Index of the ball.


	.. cpp:function:: void SetBall(const Lib3MF_uint32 nIndex, const sBall & BallInfo)

		Sets the index and radius of a single ball of a mesh object.

		:param nIndex: Index of the ball (0 to ballcount - 1). 
		:param BallInfo: filled with the ball node index and radius. 


	.. cpp:function:: void SetBalls(const CInputVector<sBall> & BallInfoBuffer)

		Sets all ball indices and radii of a mesh object.

		:param BallInfoBuffer: contains information of a number of  balls


	.. cpp:function:: void GetBalls(std::vector<sBall> & BallInfoBuffer)

		obtains all ball indices and radii of a mesh object.

		:param BallInfoBuffer: contains information of all balls 


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

