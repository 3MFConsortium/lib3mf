.. Documentation for the C++-binding of the 3MF library


C++-language bindings
===================================================================================

This space describes the usage of lib3mf in a C++ host application.


The C++-language bindings come in two different flavors:

.. tabs::
	
	.. group-tab:: Cpp
	
		If you include the header ``Cpp/lib3mf_implicit.hpp``, lib3mf will be loaded dynamically during *load-time*
		of your host application through your operating system's mechanism for loading libraries.
		
		.. code-block:: c
		
			Lib3MF::PWrapper wrapper = CWrapper::loadLibrary();
				
		The shared library file ``lib3mf.*``. needs to reside in a path that your operating systems checks when loading libraries.
		
	.. group-tab:: CppDynamic
	
		If you include the header ``CppDynamic/lib3mf_dynamic.hpp``, Lib3MF will be loaded dynamically during *run-time* of your host application through an
		explicit call to
		
		.. tabs::

			.. group-tab:: Linux

				.. code-block:: c
		
					Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary("LibraryLocation/lib3mf.so");

			.. group-tab:: Mac OSX
				
				.. code-block:: c

					Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary("LibraryLocation/lib3mf.dylib");

			.. group-tab:: Windows

				.. code-block:: c

					Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary("LibraryLocation/lib3mf.dll");
		
		i.e. you need to explicitly provide the location of the shared library file ``lib3mf.*``.

The ``Lib3MF::PWrapper`` object provides access to all functionality within lib3mf.

Both flavors of the C++-bindings are header-only which makes it extremly easy to include them into existing projects:



Minimal Example Project
-------------------------

Minimal application code:

.. tabs::

	.. group-tab:: Cpp
		
		.. literalinclude:: Lib3MF_example_implicit.cpp
			:language: cpp
			:emphasize-lines: 2,9
			
	.. group-tab:: CppDynamic
	
		.. literalinclude:: Lib3MF_example_dynamic.cpp
			:language: cpp
			:emphasize-lines: 2,9-10

CMakeLists.txt for minimal project:

.. tabs::

	.. group-tab:: Cpp
		
		.. literalinclude:: CMakeLists_implicit.txt
			:language: cmake
	
	.. group-tab:: CppDynamic
		
		.. literalinclude:: CMakeLists_dynamic.txt
			:language: cmake


The examples in the Cpp/CppDynamic-folders of the binary SDK follow exactly this pattern.


The remainder of this space is an in-depth API-reference for the functionality of lib3mf.


General Information
--------------------
.. toctree::
	:maxdepth: 2
	
	lib3mf
	lib3mf-types



API-Classes
--------------------
.. toctree::
	:maxdepth: 2
	:glob:
	
	lib3mf_*

