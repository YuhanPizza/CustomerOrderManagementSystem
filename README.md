# Welcome to the Shopping System Client

Hey there 👋! This C++ project is your go-to client-side application for a shopping system that's fully integrated with an Oracle database. Below is a quick rundown of what's under the hood.

## 🛠 Main Components

- **ShoppingCart Struct**: Think of it as your virtual cart where you stash all the products you're interested in.
- **Connection and Environment Variables**: These guys make sure we can chat with the Oracle database smoothly.
- **Main and Sub-Menus**: Simple and intuitive UI for you to navigate through login and customer service options.

## 📚 Main Functions

- **mainMenu() and subMenu()**: These show you your basic choices—log in, customer service, and so on.
- **customerService()**: Once you're in, here's where you can place an order, check its status, or even cancel it.
- **displayOrderStatus()**: Feeling anxious? Check how far along your order is!
- **cancelOrder()**: Changed your mind? No worries—just cancel it right here.
- **createEnvironment() and terminateEnvironment()**: These set up and tear down your connection to the Oracle database.
- **openConnection() and closeConnection()**: Self-explanatory, really—get connected and then say goodbye.
- **customerLogin()**: This function checks if you're in the database before letting you in.
- **addToCart()**: See something you like? Add it to your cart!
- **displayProducts()**: Peek inside your cart whenever you like.
- **checkout()**: Ready to make it official? This will process your order and update the database.

## 📂 Database Interactions

- **find_customer Procedure**: This helps `customerLogin()` confirm you're an existing customer.
- **find_product Procedure**: Used by `addToCart()` to find out how much you need to pay for that awesome product.

🚨 Note: The code seems to be a work in progress—so don't be surprised if some functions like `findProduct()` or `checkout()` are still getting dressed (i.e., they're called but not fully implemented).

## 🛡 Error Handling

We've got try-catch blocks in place for dealing with any SQL mishaps, so rest easy!

## 🎬 Program Flow

1. You're greeted with a main menu: Login or Exit.
2. If you opt to login, the program checks if you're in the database.
3. If you are, welcome aboard! You'll see a customer service menu where you can place, check, or cancel your orders.

That's the grand tour! Feel free to dive in and explore. Happy shopping! 🛒
