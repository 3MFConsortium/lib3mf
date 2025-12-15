
CVolumeDataComposite
====================================================================================================


.. cpp:class:: Lib3MF::CVolumeDataComposite : public CBase 

	




	.. cpp:function:: PBaseMaterialGroup GetBaseMaterialGroup()

		Returns the BaseMaterialGroup used within this volume data item

		:returns: The BaseMaterialGroup instance of this VolumeDataComposite element


	.. cpp:function:: void SetBaseMaterialGroup(classParam<CBaseMaterialGroup> pBaseMaterialGroupInstance)

		Sets the BaseMaterialGroup to use within this volume data item.

		:param pBaseMaterialGroupInstance: The new BaseMaterialGroup instance of this VolumeDataComposite element 


	.. cpp:function:: Lib3MF_uint32 GetMaterialMappingCount()

		Returns the number of material mappings of this VolumeDataComposite element

		:returns: the number of material mappings.


	.. cpp:function:: PMaterialMapping GetMaterialMapping(const Lib3MF_uint32 nIndex)

		Returns MaterialMappting with given index

		:param nIndex: Index of the MaterialMapping in question. 
		:returns: MaterialMapping used in this element


	.. cpp:function:: PMaterialMapping AddMaterialMapping(const sTransform & Transform)

		Adds a the MaterialMapping

		:param Transform: new transformation matrix 
		:returns: The new MaterialMapping


	.. cpp:function:: void RemoveMaterialMapping(const Lib3MF_uint32 nIndex)

		Removes the MaterialMapping with given index

		:param nIndex: The index of the MaterialMapping to be removed. 


.. cpp:type:: std::shared_ptr<CVolumeDataComposite> Lib3MF::PVolumeDataComposite

	Shared pointer to CVolumeDataComposite to easily allow reference counting.

