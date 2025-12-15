
CImplicitFunction
====================================================================================================


.. cpp:class:: Lib3MF::CImplicitFunction : public CFunction 

	




	.. cpp:function:: std::string GetIdentifier()

		Retrieves the identifier of the function

		:returns: the identifier


	.. cpp:function:: void SetIdentifier(const std::string & sIdentifier)

		Sets the identifier of the function

		:param sIdentifier: the identifier 


	.. cpp:function:: PImplicitNode AddNode(const eImplicitNodeType eNodeType, const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a node

		:param eNodeType: the type of the node 
		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PSinNode AddSinNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a SinNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PCosNode AddCosNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a CosNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PTanNode AddTanNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a TanNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PArcSinNode AddArcSinNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a ArcSinNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PArcCosNode AddArcCosNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a ArcCosNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PArcTan2Node AddArcTan2Node(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a ArcTan2Node

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PSinhNode AddSinhNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a SinhNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PCoshNode AddCoshNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a CoshNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PTanhNode AddTanhNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a TanhNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PRoundNode AddRoundNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a RoundNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PCeilNode AddCeilNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a CeilNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PFloorNode AddFloorNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a FloorNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PSignNode AddSignNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a SignNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PFractNode AddFractNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a FractNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PAbsNode AddAbsNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a AbsNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PExpNode AddExpNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a ExpNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PLogNode AddLogNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a LogNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PLog2Node AddLog2Node(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a Log2Node

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PLog10Node AddLog10Node(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a Log10Node

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PLengthNode AddLengthNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a LengthNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PTransposeNode AddTransposeNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a TransposeNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PInverseNode AddInverseNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a InverseNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PSqrtNode AddSqrtNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a SqrtNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PResourceIdNode AddResourceIdNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a ResourceIdNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PAdditionNode AddAdditionNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add an AdditionNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PSubtractionNode AddSubtractionNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a SubtractionNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PMultiplicationNode AddMultiplicationNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a MultiplicationNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PDivisionNode AddDivisionNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a DivisionNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PDotNode AddDotNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a DotNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PCrossNode AddCrossNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a CrossNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PMatVecMultiplicationNode AddMatVecMultiplicationNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a MatVecMultiplicationNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PMinNode AddMinNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a MinNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PMaxNode AddMaxNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a MaxNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PFmodNode AddFmodNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a FmodNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PPowNode AddPowNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a PowNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PSelectNode AddSelectNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a SelectNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PClampNode AddClampNode(const std::string & sIdentifier, const eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)

		Add a ClampNode

		:param sIdentifier: the identifier of the node 
		:param eConfiguration: the configuration of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PComposeVectorNode AddComposeVectorNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a ComposeVectorNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PVectorFromScalarNode AddVectorFromScalarNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a VectorFromScalar

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PDecomposeVectorNode AddDecomposeVectorNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a DecomposeVectorNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PComposeMatrixNode AddComposeMatrixNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a ComposeMatrixNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PMatrixFromRowsNode AddMatrixFromRowsNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a MatrixFromRowsNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PMatrixFromColumnsNode AddMatrixFromColumnsNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a MatrixFromColumnsNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PConstantNode AddConstantNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a ConstantNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PConstVecNode AddConstVecNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a ConstVecNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PConstMatNode AddConstMatNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a ConstMatNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PMeshNode AddMeshNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a MeshNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PUnsignedMeshNode AddUnsignedMeshNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a UnsignedMeshNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PBeamLatticeNode AddBeamLatticeNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a BeamLatticeNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PFunctionGradientNode AddFunctionGradientNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a FunctionGradientNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PNormalizeDistanceNode AddNormalizeDistanceNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a NormalizeDistanceNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PFunctionCallNode AddFunctionCallNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)

		Add a FunctionCallNode

		:param sIdentifier: the identifier of the node 
		:param sDisplayName: the display name of the node 
		:param sTag: the tag of the node 
		:returns: the added node


	.. cpp:function:: PNodeIterator GetNodes()

		Retrieves the nodes

		:returns: iterator for the list of nodes


	.. cpp:function:: void RemoveNode(classParam<CImplicitNode> pNode)

		Removes a node

		:param pNode: The node to be removed 


	.. cpp:function:: void AddLink(classParam<CImplicitPort> pSource, classParam<CImplicitPort> pTarget)

		Add a link

		:param pSource: the source port 
		:param pTarget: the target port 


	.. cpp:function:: void AddLinkByNames(const std::string & sSource, const std::string & sTarget)

		Add a link

		:param sSource: name of the source port in the format nodename.portname 
		:param sTarget: name of the target port in the format nodename.portname 


	.. cpp:function:: void Clear()

		Clears the function



	.. cpp:function:: void SortNodesTopologically()

		Sorts the nodes topologically



.. cpp:type:: std::shared_ptr<CImplicitFunction> Lib3MF::PImplicitFunction

	Shared pointer to CImplicitFunction to easily allow reference counting.

