package grpc

import (
	"context"
	"fmt"
	"log"
	"net"

	pb "recipes/server/grpc/protos"

	"recipes/server/database"

	"google.golang.org/grpc"
)

type recipeServer struct {
	db *database.Database
	pb.UnimplementedRecipesServer
}

// func (s *recipeServer) Recipe(ctx context.Context, id *pb.RecipeId) (*pb.RecipeRet, error) {
// 	return nil, nil
// }

func (s *recipeServer) Recipes(ctx context.Context, offset *pb.Offset) (*pb.RecipeListRet, error) {
	recipes, err := s.db.GetRecipes(10, int(offset.Offset))
	if err != nil {
		return nil, err
	}
	var recipeRetList pb.RecipeListRet
	recipeRetList.Recipes.Offset = int64(offset.Offset)
	for _, recipe := range recipes {
		recipeRetList.Recipes.Recipes = append(recipeRetList.Recipes.Recipes, &pb.Recipe{Name: recipe.Title})
	}
	return &recipeRetList, nil
}

func newServer(db *database.Database) *recipeServer {
	s := &recipeServer{db: db} // TODO add database pointer when grpc basic stuff is workng
	return s
}

func Start(db *database.Database) error {
	lis, err := net.Listen("tcp", fmt.Sprintf("localhost:%d", 4711))
	if err != nil {
		log.Fatalf("failed to listen: %v", err)
		return err
	}
	var opts []grpc.ServerOption
	// if *tls {
	// 	if *certFile == "" {
	// 		*certFile = data.Path("x509/server_cert.pem")
	// 	}
	// 	if *keyFile == "" {
	// 		*keyFile = data.Path("x509/server_key.pem")
	// 	}
	// 	creds, err := credentials.NewServerTLSFromFile(*certFile, *keyFile)
	// 	if err != nil {
	// 		log.Fatalf("Failed to generate credentials: %v", err)
	// 	}
	// 	opts = []grpc.ServerOption{grpc.Creds(creds)}
	// }
	grpcServer := grpc.NewServer(opts...)
	pb.RegisterRecipesServer(grpcServer, newServer(db))
	grpcServer.Serve(lis)
	return nil
}
