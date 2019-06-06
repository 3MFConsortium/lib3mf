
CBuildItemIterator
====================================================================================================


.. cpp:class:: Lib3MF::CBuildItemIterator : public CBase 

	




	.. cpp:function:: bool MoveNext()

		Iterates to the next build item in the list.

		:returns: Iterates to the next build item in the list.


	.. cpp:function:: bool MovePrevious()

		Iterates to the previous build item in the list.

		:returns: Iterates to the previous build item in the list.


	.. cpp:function:: PBuildItem GetCurrent()

		Returns the build item the iterator points at.

		:returns: returns the build item instance.


	.. cpp:function:: PBuildItemIterator Clone()

		Creates a new build item iterator with the same build item list.

		:returns: returns the cloned Iterator instance


.. cpp:type:: std::shared_ptr<CBuildItemIterator> Lib3MF::PBuildItemIterator

	Shared pointer to CBuildItemIterator to easily allow reference counting.

