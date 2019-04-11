
CCompositeMaterials
====================================================================================================


.. cpp:class:: Lib3MF::CCompositeMaterials : public CResource 

	




	.. cpp:function:: Lib3MF_uint32 GetCount()

		Retrieves the count of Composite-s in the CompositeMaterials.

		:returns: returns the count of Composite-s


	.. cpp:function:: void GetAllPropertyIDs(std::vector<Lib3MF_uint32> & PropertyIDsBuffer)

		returns all the PropertyIDs of all Composite-Mixing Values in this CompositeMaterials

		:param PropertyIDsBuffer: PropertyID of the Composite-Mixing Values in the CompositeMaterials. 


	.. cpp:function:: PBaseMaterialGroup GetBaseMaterialGroup()

		Obtains the BaseMaterialGroup instance of this CompositeMaterials.

		:returns: returns the BaseMaterialGroup instance of this CompositeMaterials


	.. cpp:function:: Lib3MF_uint32 AddComposite(const CInputVector<sCompositeConstituent> & CompositeBuffer)

		Adds a new Composite-Mixing Values to the CompositeMaterials.

		:param CompositeBuffer: The Composite Constituents to be added as composite 
		:returns: returns new PropertyID of the new Composite in the CompositeMaterials.


	.. cpp:function:: void RemoveComposite(const Lib3MF_uint32 nPropertyID)

		Removes a Composite-Maxing Ratio from the CompositeMaterials.

		:param nPropertyID: PropertyID of the  Composite-Mixing Values in the CompositeMaterials to be removed. 


	.. cpp:function:: void GetComposite(const Lib3MF_uint32 nPropertyID, std::vector<sCompositeConstituent> & CompositeBuffer)

		Obtains a Composite-Maxing Ratio of this CompositeMaterials.

		:param nPropertyID: the PropertyID of the Composite-Maxing Ratio in the CompositeMaterials. 
		:param CompositeBuffer: The Composite-Mixing Values with the given PropertyID 


.. cpp:type:: std::shared_ptr<CCompositeMaterials> Lib3MF::PCompositeMaterials

	Shared pointer to CCompositeMaterials to easily allow reference counting.

