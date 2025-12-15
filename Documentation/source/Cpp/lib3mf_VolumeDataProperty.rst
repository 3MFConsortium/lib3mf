
CVolumeDataProperty
====================================================================================================


.. cpp:class:: Lib3MF::CVolumeDataProperty : public CFunctionReference 

	




	.. cpp:function:: std::string GetName()

		Gets the qualified name of this property.

		:returns: The qualified name of this property.


	.. cpp:function:: void SetIsRequired(const bool bIsRequired)

		Sets whether this property is required to process this 3MF document instance.

		:param bIsRequired: New value for whether this property is required to process this 3MF document instance. 


	.. cpp:function:: bool IsRequired()

		Returns whether this property is required to process this 3MF document instance.

		:returns: Is this property required to process this 3MF document instance?


.. cpp:type:: std::shared_ptr<CVolumeDataProperty> Lib3MF::PVolumeDataProperty

	Shared pointer to CVolumeDataProperty to easily allow reference counting.

