
CImage3D
====================================================================================================


.. cpp:class:: Lib3MF::CImage3D : public CResource 

	




	.. cpp:function:: std::string GetName()

		returns the name of this Image3D

		:returns: the name of this Image3D


	.. cpp:function:: void SetName(const std::string & sName)

		sets a new name of this Image3D

		:param sName: the new name of this Image3D 


	.. cpp:function:: bool IsImageStack()

		Retrieves, if this Image3D is a ImageStack

		:returns: returns, whether the Image3D is an ImageStack


.. cpp:type:: std::shared_ptr<CImage3D> Lib3MF::PImage3D

	Shared pointer to CImage3D to easily allow reference counting.

