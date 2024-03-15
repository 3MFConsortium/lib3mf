package main

import (
	"fmt"
	"log"
	"../Go/go_bindings"
)

func investigateModel(model* lib3mf.Model) {
	{
		buildItems, err := model.GetBuildItems()
		if (err != nil) {
			log.Fatal(err)
		}
		nBuildItems,_ := buildItems.Count()
		fmt.Printf("The file contains %d build items:\n", nBuildItems)
		iBuildItems := 0
		ret, _ := buildItems.MoveNext()
		for  ret {
			buildItem, err := buildItems.GetCurrent()
			if (err != nil) {
				log.Fatal(err)
			}
			_, err = buildItem.GetObjectResource()
			if (err != nil) {
				log.Fatal(err)
			}
			partNumber, err := buildItem.GetPartNumber()
			if (err != nil) {
				log.Fatal(err)
			}
			fmt.Printf("  Build item %d: buildnumber=\"%s\"\n", iBuildItems, partNumber)

			ret, _ = buildItems.MoveNext()
			iBuildItems++
		}
		
	}
	
	
	{
		objects, err := model.GetObjects()
		if (err != nil) {
			log.Fatal(err)
		}
		nObjects,_ := objects.Count()
		fmt.Printf("The file contains %d objects:\n", nObjects)
		iObjects := 0
		ret, _ := objects.MoveNext()
		for  ret {
			object, err := objects.GetCurrentObject()
			if (err != nil) {
				log.Fatal(err)
			}
			name, err := object.GetName()
			if (err != nil) {
				log.Fatal(err)
			}
			partNumber, err := object.GetPartNumber()
			if (err != nil) {
				log.Fatal(err)
			}
			fmt.Printf("  Object %d: Name =\"%s\", buildnumber=\"%s\"\n", iObjects, name, partNumber)

			ret, _ = objects.MoveNext()
			iObjects++
		}
	}
}

func main() {
	wrapper, err := lib3mf.LoadLibrary("../../../build/Debug/lib3mf.dll")
	if (err != nil) {
		log.Fatal(err)
	}
	
	nMajor, nMinor, nMicro, err := wrapper.GetLibraryVersion()
	if (err != nil) {
		log.Fatal(err)
	}
	versionString := fmt.Sprintf("lib3mf.version = %d.%d.%d", nMajor, nMinor, nMicro)

	hasInfo, preReleaseInfo, err := wrapper.GetPrereleaseInformation()
	if (err != nil) {
		log.Fatal(err)
	}
	if (hasInfo) {
		versionString += "-"+preReleaseInfo
	}
	
	hasInfo, buildInfo, err := wrapper.GetBuildInformation()
	if (err != nil) {
		log.Fatal(err)
	}
	if (hasInfo) {
		versionString += "+"+buildInfo
	}

	fmt.Println(versionString)


	model, err := wrapper.CreateModel()
	if (err != nil) {
		log.Fatal(err)
	}

	reader, err := model.QueryReader("3mf")
	if (err != nil) {
		log.Fatal(err)
	}

	reader.ReadFromFile("../Files/Helix.3mf")

	investigateModel(&model)
	
	fmt.Printf("Done\n")
}
