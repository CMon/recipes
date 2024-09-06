package database

import (
	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
)

type Unit struct {
	gorm.Model
	Abbreviation string `gorm:"unique"`
	CompleteName string
}

type Ingredient struct {
	gorm.Model
	Name string `gorm:"unique"`
	//   cereal::make_nvp("foodCategory", foodCategory_),
	IsLiquid        bool
	ContainsGluten  bool
	ContainsLactose bool

	IngredientPODID uint // back ref to the pod
}

type IngredientPOD struct {
	gorm.Model
	Count        uint
	UnitId       uint
	Unit         Unit `gorm:"foreignKey:UnitId"`
	IngredientId uint
	Ingredient   Ingredient `gorm:"foreignKey:IngredientId"`
	IsOptional   bool
	RecipeId     uint // as back ref id to the 1:n connection
}

type PortionUnit struct {
	gorm.Model
	Name string `gorm:"unique"`
}

type PortionPOD struct {
	gorm.Model
	PortionUnitID uint
	PortionUnit   PortionUnit `gorm:"foreignKey:PortionUnitID"`
	Count         uint
	RecipeId      uint // as back ref id to the 1:n connection
}

type Step struct {
	gorm.Model
	Description string
	RecipeId    uint // as back ref id to the 1:n connection
}

type Recipe struct {
	gorm.Model
	Title       string
	Steps       []Step
	Portions    PortionPOD
	Ingredients []IngredientPOD
}

type database struct {
	dbFile string
	dbConn *gorm.DB
}

func NewDatabaseConnection(databaseFile string) (database, error) {
	dbConn, err := gorm.Open(sqlite.Open(databaseFile), &gorm.Config{})
	if err != nil {
		panic("failed to connect database")
	}
	db := database{
		dbFile: databaseFile,
		dbConn: dbConn,
	}
	return db, err
}

func (db database) InitDatabase() error {
	// Migrate the schema
	db.dbConn.AutoMigrate(&Unit{})
	db.dbConn.AutoMigrate(&Ingredient{})
	db.dbConn.AutoMigrate(&IngredientPOD{})
	db.dbConn.AutoMigrate(&Step{})
	db.dbConn.AutoMigrate(&PortionUnit{})
	db.dbConn.AutoMigrate(&PortionPOD{})
	db.dbConn.AutoMigrate(&Recipe{})

	// // Create
	// db.Create(&Product{Code: "D42", Price: 100})
	//
	// // Read
	// var product Product
	// db.First(&product, 1) // find product with integer primary key
	// db.First(&product, "code = ?", "D42") // find product with code D42
	//
	// // Update - update product's price to 200
	// db.Model(&product).Update("Price", 200)
	// // Update - update multiple fields
	// db.Model(&product).Updates(Product{Price: 200, Code: "F42"}) // non-zero fields
	// db.Model(&product).Updates(map[string]interface{}{"Price": 200, "Code": "F42"})
	//
	// // Delete - delete product
	// db.Delete(&product, 1)
	return nil
}

func (db database) AddDebugData() error {
	garlic := Ingredient{Name: "Garlic", IsLiquid: false, ContainsGluten: false, ContainsLactose: false}
	chili := Ingredient{Name: "Chili", IsLiquid: false, ContainsGluten: false, ContainsLactose: false}

	db.dbConn.Create(&garlic)
	db.dbConn.Create(&chili)

	oneGram := Unit{CompleteName: "Gram", Abbreviation: "g"}
	onePiece := Unit{CompleteName: "Piece", Abbreviation: "pc"}
	oneClove := Unit{CompleteName: "Clove", Abbreviation: "cl"}
	oneMilliliter := Unit{CompleteName: "Milliliter", Abbreviation: "ml"}
	db.dbConn.Create(&oneGram)
	db.dbConn.Create(&onePiece)
	db.dbConn.Create(&oneClove)
	db.dbConn.Create(&oneMilliliter)

	glass := PortionUnit{Name: "Glass"}
	db.dbConn.Create(&glass)

	garlicIngredient := IngredientPOD{Count: 10, UnitId: oneClove.ID, IngredientId: garlic.ID, IsOptional: false}
	chiliIngredient := IngredientPOD{Count: 20, UnitId: onePiece.ID, IngredientId: chili.ID, IsOptional: false}
	hotSauce := Recipe{
		Title:    "Hot sauce",
		Portions: PortionPOD{PortionUnitID: glass.ID, Count: 4},
		Steps: []Step{
			Step{Description: "mix"},
			Step{Description: "eat"},
		},
		Ingredients: []IngredientPOD{
			garlicIngredient,
			chiliIngredient,
		},
	}
	db.dbConn.Create(&hotSauce)

	return nil
}
