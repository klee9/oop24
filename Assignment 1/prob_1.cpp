#ifndef IIKH_H
#define IIKH_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

// class for ingredients used in a recipe
class Ingredient {
public:
    
    /*
     * constructor: takes the name, unit of measurement, and the quantity of the ingredient as input parameters
     */
    
    Ingredient(string name, string unit, double quantity);
    
    /*
     * change_unit: changes the selected ingredient's unit of measurement (e.g., from grams to ml)
     * convert_unit: converts the unit (e.g., g to kg, ml to l)
     */
    
    void change_unit(string name);
    void convert_unit(string new_unit);

private:
    
    /*
     * name: name of the ingredient
     * unit: unit of measurement (e.g., "g", "oz", "ml")
     * quantity: quantity of ingredient
     */
    
    string name;
    string unit;
    double quantity;
};

// class for recipes
class Recipe {
public:
    
    /*
     * constructor: takes the name, list of ingredients, cooking instructions, amount of servings and preparation time as input parameters
     */
    
    Recipe(string recipe_name, vector<Ingredient> ingredients, string instructions, int servings, int prep_time);
    
    /*
     * add_ingredient: adds an ingredient to the recipe
     * remove_ingredient: removes an ingredient from the recipe
     * update_ingredient_quantity: modifies the quantity of an ingredient
     * set_instructions: sets the instructions for the recipe
     * display_recipe: displays a recipe with its picture and instructions included
     * get_instructions: returns the instructions for the recipe
     * get_recipe_name: returns the name of the recipe (food name)
     * get_ingredient: returns a list of ingredients of the recipe
     */
    
    void add_ingredient(Ingredient ingredient);
    void remove_ingredient(string ingredient_name);
    void update_ingredient_quantity(string ingredient_name, double new_quantity);
    void set_instructions(string instructions);
    void display_recipe(string name);
    
    vector<Ingredient> get_ingredients();
    string get_recipe();
    string get_recipe_name();

private:
    
    /*
     * servings: number of servings
     * prep_time: preparation time for the recipe
     * recipe_name: name of the recipe (food name)
     * instructions: set of instructions for the recipe
     * ingredients: a list of ingredients in the recipe
     */
    
    int servings;
    int prep_time;
    string recipe_name;
    string instructions;
    vector<Ingredient> ingredients;
};

// class for a meal plan
class MealPlan {
public:
    
    /*
     * constructor: takes name of the meal plan, type of the plan, and a list of recipes in the meal plan as input parameters
     */
    
    MealPlan(string plan_name, string type, vector<Recipe> recipes);

    /*
     * add_recipe: adds a recipe to the meal plan
     * remove_recipe: removes a recipe from the meal plan
     * update_recipe: update recipe details in a meal plan
     * display_meal_plan: searches for the meal plan from the database and returns the meal plan and the grocery list.
     * get_recipes: returns recipes of the meal plan
     * get_grocery_list: returns a grocery list for a meal plan
     */
    
    void add_recipe(Recipe recipe, string type);
    void remove_recipe(string recipe_name, string type);
    void update_recipe(string recipe_name, Recipe new_recipe);
    void display_meal_plan(string plan_name);
    
    vector<Recipe> get_recipes(string plan_name);
    vector<Ingredient> get_grocery_list(string plan_name);

private:
    
    /*
     * type: type of the meal plan (e.g., "weekly", "dinner")
     * plan_name: name of the meal plan (e.g., "John's Meal Plan")
     * recipes: list of recipes in the meal plan
     */
    
    string type;
    string plan_name;
    vector<Recipe> recipes;
};

// class for the database of recipes and meal plans
class RecipeDB {
public:
    
    /*
     * constructor: loads itself upon creation
     */
    RecipeDB();
    
    /*
     * add_recipe: adds a recipe to the database
     * remove_recipe: removes a recipe from the database
     * sort_recipe: sorts recipes according to the option (e.g., "ascending", "descending", "category")
     * display_all_recipes: displays all recipes in the database
     * list_meal_plans: lists all available meal plans
     * save_to_file: saves the current database to a file
     * load_from_file: loads the last save from a file
     * search_recipe: returns a specified recipe
     */
    
    void add_recipe(Recipe recipe);
    void remove_recipe(string recipe_name);
    void sort_recipe(string option);
    void display_all_recipes();
    void list_meal_plans();
    void save_to_file();
    void load_from_file();
    
    Recipe search_recipe(string recipe_name);
    
private:
    
    /*
     * recipes: collection of recipes in the database
     * sort_option: sorting option (e.g., "ascending", "descending")
     */
    
    vector<Recipe> recipes;
    string sort_option;
};

// class for the display(UI) of the IIKH system
class IIKHDisplay {
public:
    
    /*
     * constructor: opens a new window and displays a welcome message
     */
    
    IIKHDisplay();
    
    /*
     * display_title_screen: displays a welcome messages and a start button
     * display_main_menu: displays the main menus
     * display_recipes_menu: displays the recipes menu along with available selections
     * display_plans_menu: displays the meal plans menu along with available selections
     */
    
    void display_title_screen()
    void display_main_menu();
    void display_recipes_menu();
    void display_plans_menu();
};

#endif
