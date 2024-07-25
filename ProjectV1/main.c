#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include <time.h>

#define COLOR_BOLD_SLOW_BLINKING      "\e[1;5m"
#define COLOR_BOLD_SLOW_BLINKING_RED  "\e[1;5;31m"
#define COLOR_BOLD_BLUE               "\e[1;34m"

typedef struct Category Category;
typedef struct Product Product;


COORD coord={0,0};                   // this is global variable
                                    //center of axis is set to the top left cornor of the screen
 void gotoxy(int x,int y)
 {
   coord.X=x;
   coord.Y=y;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
 }


void SetColor(int ForgC)
 {
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

void PrintDots()
{
    char dots = '-';

    int index;
    for(index = 0; index <= 5; index++)
    {   SetColor(2);
        Sleep(100);
        printf("%c", dots);
    }
}

void Welcome()
{
    printf("\t\t\t\t\t");
    PrintDots();
    printf(" || WELCOME || ");
    PrintDots();
    Sleep(400);
    printf("\n\t\t\t\t\t===========================\n");
    SetColor(7);
    Sleep(500);
};

void WelcomeWithoutSleep()
{
    SetColor(2);
    printf("\t\t\t\t\t");
    printf("------ || WELCOME || ------");
    printf("\n\t\t\t\t\t===========================");
    SetColor(7);
    Sleep(500);
};

void PrintMenu(char menu[][20])
{
    int opt = 0;
    int y = 4;
    int index;
    for(index = 0; index < 8; index++)
        {
            gotoxy(45,y);
            printf("%s", menu[index]);
            y++;
        };
    printf("\n");
    gotoxy(0,y);
};


struct Product
{
    //char cat_name[20];
    //int cat_id;

    char name_product[20];
    int price, quantity;
};

struct Category
{
    char name[20];
    int CurrentProduct;
    int ProductSize;
    //pointer that point to array of products
    Product *Pointer2Product;
};


/*void CheckProuducts(Category *c)
{
    if(c.CurrentProduct < c.ProductSize-1)
    {
        return 0;
    }
    else
    {
        Product *New = malloc(sizeof(product)*(c.ProductSize)*2);
        int i;
        for(i = 0; i < c.ProductSize; i++)
        {
            New[i] = c.Pointer2Product[i];
        }
        c.ProductSize*=2;
        free(Pointer2Product);
        c.Pointer2Product = New;
    }
}
*/

void CheckCategories(int *CatSize, int *CurrentCategory, Category *Store)
{
    if((*CurrentCategory) < (*CatSize)-1)
    {
        (*CurrentCategory)++;
    }
    else
    {
        Category *NewSize = malloc(sizeof(Category)*(*CatSize)*2);
        int i;
        for(i = 0; i < *CatSize; i++)
        {
            NewSize[i]=Store[i];
        };
        *CatSize*=2;
        free(Store);
        //to hold the address of new array
        Store=NewSize;
        (*CurrentCategory)++;
    };

};

Category CreateCategory()
{

    Category c;

    puts("Enter name of Category:");
    scanf("%s", &c.name);

    c.ProductSize = 10;

    c.Pointer2Product = malloc(sizeof(Product) * c.ProductSize);


    //To Begin add products from 0 index in their array
    c.CurrentProduct=0;
    return c;

};

Product CreateProduct()
{
    Product p;

    puts("\nEnter name of product:");
    scanf("%19s", p.name_product);

    printf("\nEnter the price of the product:");

    do
    {
        if(scanf("%d", &p.price)==1 && p.price>0)
            break;
        else{
            printf("\nPlease enter valid price of the product:");
            while(getchar() != '\n');
         }
    }
    while(1);

    printf("\nEnter quantity of the product:");

    do
    {
        if(scanf("%d", &p.quantity) == 1 && p.quantity > 0)
        {
            break;
        }
        else
        {
            printf("\nPlease enter valid quantity of the product:");
            while(getchar() != '\n');
        }
    }
    while(1);

    return p;
};

void AddProduct(Category *c)
{
    char key;
    do
    {
        while(c->CurrentProduct == (c->ProductSize)-1)
        {
            Product *New = malloc(sizeof(Product)*(c->ProductSize)*2);
            int i;
            for(i = 0; i < c->ProductSize; i++)
            {
                New[i] = c->Pointer2Product[i];
            }
            c->ProductSize*=2;
            free(c->Pointer2Product);
            c->Pointer2Product = New;
        }
        c->Pointer2Product[c->CurrentProduct] = CreateProduct();
        c->CurrentProduct++;
        printf("Do you want to add products again (Y|N)\n");
        key = getche();
    }
    while(key == 'Y' || key == 'y');
};

void DisplayCategory(Category *c)
{
    //why not work in c ?
    //Category c = *this;
    //CurrentCategory++; //To increase index of category array
    int counter;
    printf("\nCategory name:%s\n====================\n",c->name);
    if(c->CurrentProduct == 0)
    {
        puts("There in no products in this category till now...");
    }
    else
    {
        for(counter = 0; counter < (c->CurrentProduct); counter++)
        {

            printf("Product Name: %s\t\t Price: %d\t Quantity: %d\n",c->Pointer2Product[counter].name_product, c->Pointer2Product[counter].price, c->Pointer2Product[counter].quantity);
        }
    }
    printf("\n");
};

void EnteredCategory()
{

}


void CheckAfterOrder(Category *c, Product *p)
{
    // Quantity be zero when move or order total q
    if(p->quantity == 0)
    {
        int test = c->CurrentProduct;
        Product *NewPointer = malloc(sizeof(Product)*(c->ProductSize));
        int i = 0;
        int inew = 0;
        do
        {
            if(c->Pointer2Product[i].quantity > 0)
            {
                NewPointer[inew] = c->Pointer2Product[i];
                i++;
                inew++;
            }
            else {
                i++;
            }

        }
        while(i < c->CurrentProduct);

        (c->CurrentProduct)=(inew);
        free(c->Pointer2Product);
        c->Pointer2Product = NewPointer;
    }
}

int ChooseCategory(int *CurrentCategory, Category *Store)
{
    if(*CurrentCategory == -1)
        {
            printf("There is no category till now!\n");
            return -1;
        }
        else
        {
            printf("\n\nCategories are:\n===================\n");
            int index;
            for(index=0; index <= *CurrentCategory; index++)
            {
                printf("%d-%s\n", index+1, Store[index].name);
            }
            printf("\n");
            printf("Choose category by number:");
            int num;
            scanf("%d", &num);
            return num;
        }

}

void DisplayProducts(Category *Store, int *CurrentCategory)
{
    int counter;
    for(counter = 0; counter <= (*CurrentCategory); counter++)
    {
        printf("\nCategory name:%s\n====================\n",Store[counter].name);
        for(int i = 0; i < (Store[counter].CurrentProduct); i++)
        {
            printf("Product Name: %s\t\t Price: %d\t Quantity: %d\n",Store[counter].Pointer2Product[i].name_product, Store[counter].Pointer2Product[i].price, Store[counter].Pointer2Product[i].quantity);
        }
    }
    printf("\n");
    //int test = c->CurrentProduct;
  /*  int index;
    for(index=0; index < c->CurrentProduct; index++)
    {
        printf("%d-%s\n", index+1, c->Pointer2Product[index].name_product);
    }*/
}

Product *ChooseProduct(Category *c)
{

        printf("The products of category (%s)\n==========================\n", c->name);
        int i;
        for(i = 0; i < c->CurrentProduct; i++)
        {
            printf("%d-%s\n", i+1, c->Pointer2Product[i].name_product);
        }
        printf("\n");
        printf("Choose Product by number:");
        int p_num;
        scanf("%d", &p_num);
        printf("\nThe product is: %s\t\tPrice: %d\t\tQuantity: %d\n", c->Pointer2Product[p_num-1].name_product, c->Pointer2Product[p_num-1].price, c->Pointer2Product[p_num-1].quantity);
        Product *p = &c->Pointer2Product[(p_num)-1];
        return p;

};

int main()
{

    int CurrentCategory = -1;
    int CatSize = 10;
    Category *Store = malloc(sizeof(Category)*CatSize);

    int key;
    char menu[8][20] = {"1-Create Category", "2-Remove Category", "3-Add Product", "4-Remove Product", "5-Display Products", "6-Move Product", "7-Make Order", "8-Exit"};

    int opt = 0;
    Welcome();
    char ch;

    do
    {
        system("cls");
        WelcomeWithoutSleep();
        SetColor(3);

        PrintMenu(menu);
        gotoxy(30, 14);
        printf("Enter number from 1 to 8 as display in list:\n");
        SetColor(7);
        key = getche();


        switch(key)
        {
            case 49:
                system("cls");

                char Ask;
                do
                {
                    CheckCategories(&CatSize, &CurrentCategory, Store);
                    Store[CurrentCategory] = CreateCategory();
                    // ASK USER IF HE WANT TO ADD PRODUCTS AFTER MAKING CATEGORY
                    puts("Do you want to add products (Y|N)");
                    Ask = getche();

                    switch(Ask)
                    {
                        case 'Y':
                        case 'y':

                            AddProduct(&Store[CurrentCategory]);
                            DisplayCategory(&Store[CurrentCategory]);
                            //Store[CurrentCategory].Pointer2Product[Store[CurrentCategory].CurrentProduct];
                            //(Store[CurrentCategory].CurrentProduct)++;
                            break;



                        default:
                            SetColor(6);
                            printf("\n\nGreat! You now have category: (%s), but it is still empty...\n\n", Store[CurrentCategory].name);
                            SetColor(7);
                            break;
                    }
                    puts("Do you want to add another category (Y|N)");
                    Ask = getche();
                    printf("\n\n");
                }
                while(Ask == 'y' || Ask == 'Y');

                puts("\nPress any button to back to main menu...");
                getche();
                break;

            case 51:
                {
                    system("cls");
                    int num = ChooseCategory(&CurrentCategory, Store);
                    if(num != -1)
                    {
                        AddProduct(&Store[num-1]);
                        printf("\n\nCategory products after UPDATE\n================================\n");
                        DisplayCategory(&Store[num-1]);
                    }

                    puts("\nPress any button to back to main menu...");
                    getche();
                    //break;
                    break;
                }

            case 53:
                system("cls");
                if(CurrentCategory == -1)
                {
                    SetColor(6);
                    puts("There is no categories till now...");
                    SetColor(7);
                }
                else
                {
                    int index;
                    for(index = 0; index <= CurrentCategory; index++)
                    {
                        DisplayCategory(&Store[index]);
                    }
                }

                puts("\nPress any button to back to main menu...");
                getche();
                break;

            case 54:
                system("cls");
                if(CurrentCategory > 0)
                {

                    int num = ChooseCategory(&CurrentCategory, Store);
                    if(num != -1)
                    {
                        if(Store[num-1].CurrentProduct != 0)
                        {
                            Product *p = ChooseProduct(&Store[num-1]);
                            //To Copy values in temporary product
                            Product ProductCopy = *p;
                            p->quantity = 0;
                            // CheckAfterOrder make check if quantity = 0 wilh remove  product
                            CheckAfterOrder(&Store[num-1], p);
                            int num2 = ChooseCategory(&CurrentCategory, Store);
                            if(num ==  num2)
                            {
                                SetColor(6);
                                puts("Warning! You choose the same category and this will lead to only re-arrange products...");
                            }
                            SetColor(7);
                            Store[num2-1].Pointer2Product[Store[num2-1].CurrentProduct] = ProductCopy;
                            Store[num2-1].CurrentProduct++;

                            printf("\n\nCategory (%s) after UPDATE\n================================\n", Store[num2-1].name);
                            for(int index=0; index < Store[num2-1].CurrentProduct; index++)
                            {
                                printf("%d-%s\n", index+1, Store[num2-1].Pointer2Product[index].name_product);
                            }
                        }
                        else
                        {
                            SetColor(6);
                            puts("\nThere in no products in category");
                            SetColor(7);
                            puts("\n\nPress any button to back to main menu...");
                            getche();
                            break;
                        }
                    }
                }
                else
                {
                    SetColor(6);
                    puts("\nThere in less than two categories");
                    SetColor(7);
                }

                    puts("\n\nPress any button to back to main menu...");
                    getche();
                    break;

            case 55:
                {
                    system("cls");

                            int num = ChooseCategory(&CurrentCategory, Store);
                            if(num != -1)
                            {
                                if(Store[num-1].CurrentProduct != 0)
                                {
                                    Product *p = ChooseProduct(&Store[num-1]);

                                    int p_quantity = 0;
                                   /* do
                                    {
                                        printf("\nPlease Enter the quantity you want to order:");
                                        if(scanf("%d", &p_quantity) == 1 && p_quantity <= (p->quantity) && p_quantity > 0)
                                        //scanf("%d", &p_quantity);
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            printf("Please Enter the quantity you want to order:");
                                            while(getchar()!='\n');
                                            break;
                                        }
                                    }
                                    while(1);*/
                                    printf("\nPlease Enter the quantity you want to order:");

                                    do
                                    {
                                        if(scanf("%d", &p_quantity)==1 && p_quantity >0)
                                            break;
                                        else{
                                    printf("\nPlease Enter valid the quantity you want to order:");
                                            while(getchar() != '\n');
                                         }
                                    }
                                    while(1);

                                    p->quantity -= p_quantity;
                                    puts("\n\nGreat! Your order details are:\n=================================");
                                    printf("Product name: %s\t\t Quantity: %d\t\t Total price: %d\n\n\n",p->name_product, p_quantity, (p->price)*p_quantity);
                                    CheckAfterOrder(&Store[num-1], p);
                                    if(Store[num-1].CurrentProduct == 0)
                                    {
                                        printf("Category products after UPDATE\n================================\n");
                                        SetColor(6);
                                        puts("Warning! The category empty now...");
                                        SetColor(7);
                                    }
                                    else
                                    {
                                        printf("Category products after UPDATE\n================================\n");
                                            for(int index=0; index < Store[num-1].CurrentProduct; index++)
                                            {
                                                printf("%d-%s\n", index+1, Store[num-1].Pointer2Product[index].name_product);
                                            }
                                    }
                                    // For test      printf("The new quantity is %d\n\n",Store[num-1].Pointer2Product[p_num-1].quantity);
                                }
                                else
                                {
                                    SetColor(6);
                                    puts("Warning! The category empty now...");
                                    SetColor(7);
                                }

                                    puts("\n\nPress any button to back to main menu...");
                                    getche();
                                    break;

                                }
                                else
                                {

                                    puts("\nPress any button to back to main menu...");
                                    getche();
                                    break;

                                }

            case 56:
                system("cls");
                exit(0);
                }

        }
    }
    while(1);

    return 0;
}
