package main

import (
	"flag"
	"fmt"
	"recipes/server/database"
	"recipes/server/grpc"
)

func main() {
	fmt.Println("Running recipes Server")

	var databaseFileName = flag.String("database", "/tmp/test.db", "The file the sqlite db should be stored")
	var initTestData = flag.Bool("initTestData", false, "Init the database with test data")
	flag.Parse()
	// TODO:
	// create proto file to communicate with this go server
	// it should include the Recipes as usabel structs
	// start a grpc server that will handle all the stuff
	// add some argument parsing to this server so we can give in some parameters
	// add the fodcategory logic

	db, err := database.NewDatabaseConnection(*databaseFileName)
	if err != nil {
		fmt.Println(err)
		return
	}
	db.InitDatabase()

	if *initTestData {
		db.AddTestData()
	}

	grpc.Start(&db)
}
