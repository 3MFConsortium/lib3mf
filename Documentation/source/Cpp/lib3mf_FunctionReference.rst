
CFunctionReference
====================================================================================================


.. cpp:class:: Lib3MF::CFunctionReference : public CBase 

	




	.. cpp:function:: Lib3MF_uint32 GetFunctionResourceID()

		Returns the UniqueResourceID of the Function. Only functions with a 'pos'-input are allowed.

		:returns: returns the UniqueResourceID.


	.. cpp:function:: void SetFunctionResourceID(const Lib3MF_uint32 nUniqueResourceID)

		Sets the UniqueResourceID to refer to.

		:param nUniqueResourceID: UniqueResourceID of the function 


	.. cpp:function:: sTransform GetTransform()

		Returns the transformation matrix into the coordinate system of the referenced Function.

		:returns: the transformation matrix


	.. cpp:function:: void SetTransform(const sTransform & Transform)

		Sets the transformation matrix into the coordinate system of the referenced Function.

		:param Transform: new transformation matrix 


	.. cpp:function:: std::string GetChannelName()

		Returns the name of the function output channel to use.

		:returns: the name of the function output channel


	.. cpp:function:: void SetChannelName(const std::string & sChannelName)

		Sets the name of the function output channel to use.

		:param sChannelName: new name of the function output channel 


	.. cpp:function:: void SetMinFeatureSize(const Lib3MF_double dMinFeatureSize)

		Sets the minimal feature size as a hint for the function evaluator

		:param dMinFeatureSize: minimal feature size 


	.. cpp:function:: Lib3MF_double GetMinFeatureSize()

		Returns the minimal feature size as a hint for the function evaluator

		:returns: minimal feature size


	.. cpp:function:: void SetFallBackValue(const Lib3MF_double dFallBackValue)

		Sets the fallback value to use if the function evaluation fails (e.g. evaluates to NaN or Inf).

		:param dFallBackValue: fallback value 


	.. cpp:function:: Lib3MF_double GetFallBackValue()

		Returns the fallback value to use if the function evaluation fails (e.g. evaluates to NaN or Inf).

		:returns: fallback value


.. cpp:type:: std::shared_ptr<CFunctionReference> Lib3MF::PFunctionReference

	Shared pointer to CFunctionReference to easily allow reference counting.

