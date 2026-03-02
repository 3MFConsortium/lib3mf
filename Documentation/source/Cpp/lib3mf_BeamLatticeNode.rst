
CBeamLatticeNode
====================================================================================================


.. cpp:class:: Lib3MF::CBeamLatticeNode : public CImplicitNode 

	Signed distance to beam lattice




	.. cpp:function:: PImplicitPort GetInputBeamLattice()

		Retrieves the input for the model resource id of the beam lattice

		:returns: the input port for the model resource id of the beam lattice (mesh with beamlattice extension)


	.. cpp:function:: PImplicitPort GetInputPos()

		Retrieves the input for the position

		:returns: the input port for the position


	.. cpp:function:: PImplicitPort GetOutputDistance()

		Retrieves the output

		:returns: the output port for the signed distance to the beam lattice


	.. cpp:function:: void SetAccurateRange(const Lib3MF_double dAccurateRange)

		Sets the accurate range for distance computation

		:param dAccurateRange: the accurate range in model units 


	.. cpp:function:: Lib3MF_double GetAccurateRange()

		Retrieves the accurate range for distance computation

		:returns: the accurate range in model units


.. cpp:type:: std::shared_ptr<CBeamLatticeNode> Lib3MF::PBeamLatticeNode

	Shared pointer to CBeamLatticeNode to easily allow reference counting.

