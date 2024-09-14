
#include <SimpleCollections.h>
#include <SCCircularBuffer.h>
#include <stdio.h>

#define INTERRUPT_PIN 5

class Book {
    int bookId;
    const char* bookName;
    float price;

public:
    Book() : bookId(-1), bookName(), price() {}
    Book(int bookId, const char *bookName, float price) : bookId(bookId), bookName(bookName), price(price) {}
    Book(const Book&) = default;
    Book& operator=(const Book&) = default;

    [[nodiscard]] int getKey() const { return bookId; }

    [[nodiscard]] const char* getBookName() const { return bookName; }
    [[nodiscard]] float getPrice() const { return price; }
};

using namespace ioaTreeInternal;

BtreeList<int, Book> bookStore;
SCCircularBuffer byteBuffer(128);

void intAddToBuffer(uint, uint32_t) {
    static uint8_t nextByte = 0;
    byteBuffer.put(nextByte++);
}

int main() {
    stdio_init_all();

    bookStore.add(Book(1, "Getting Started with Pico", 20.99));
    bookStore.add(Book(2, "Advanced CMake", 19.99));
    bookStore.add(Book(3, "Building Amplifiers", 22.99));

    auto book2ptr = bookStore.getByKey(2);
    printf("Book 2 is %s price %f\n", book2ptr->getBookName(), book2ptr->getPrice());

    int count = bookStore.count();
    printf("Count of books\n", count);

    printf("list all books start\n");
    for(const Book& book : bookStore) {
        printf("Book %d is %s price %f\n", book.getKey(), book.getBookName(), book.getPrice());
    }

    gpio_init(INTERRUPT_PIN);
    gpio_set_dir(INTERRUPT_PIN, GPIO_IN);
    gpio_pull_up(INTERRUPT_PIN);
    gpio_set_irq_enabled_with_callback(INTERRUPT_PIN, GPIO_IRQ_EDGE_RISE, true, intAddToBuffer);

    printf("Listening for interrupt on %d\n", INTERRUPT_PIN);

    while(true) {
        if(byteBuffer.available()) {
            printf("Buffer Rx %d\n", (int)byteBuffer.get());
        }
    }
}
