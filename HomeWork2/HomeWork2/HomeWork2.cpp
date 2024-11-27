#include <iostream>
#include <cmath>
using namespace std;

class Polynomial; // forward declaration

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& poly);

private:
    float coef; // coefficient
    int exp;    // exponent
};
//先依照題目說的創建類別Polynomial(表示依個多項式，以及多個操作多項式的方法)以及Term類別(代表多項式的係數(coef)以及指數(exp)
// ，同時他也是Polynomial的朋友類別)，而main則拿來創建p1 和p2(兩個多項式)及輸出與輸入。 
class Polynomial {
public:
    Polynomial(int capacity = 10)
        : capacity(capacity), terms(0) {
        termArray = new Term[capacity];
    }

    ~Polynomial() {
        delete[] termArray;
    }
    //如果係數為零時則直接返回而下段程式用來檢查是否有與exp相同的項，如果有則加coef(兩係數相加的意思)，並檢查合併後的係數是否為 0。 為 0 就刪除該項。
    void add(float coef, int exp) {
        if (coef == 0)
            return; // Skip zero terms

        for (int i = 0; i < terms; ++i) {
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;
                if (termArray[i].coef == 0) {
                    removeTerm(i);
                }
                return;
            }
        }
    //檢查過後發現沒有相同exp的項，則直接將新項添加到陣列的未端，並更新terms成員，再對多項式進行排序。
        if (terms == capacity) {
            resize();
        }
        termArray[terms].coef = coef;
        termArray[terms].exp = exp;
        ++terms;
        sortTerms();
    }
    //給多項式帶x值並返回計算結果。 
    float evaluate(float x) const {
        float result = 0;
        for (int i = 0; i < terms; ++i) {
            result += termArray[i].coef * pow(x, termArray[i].exp);
        }
        return result;
    }
    //處理多加法多項式，將兩個多項式的每一項逐一加入到一個新的Polynomial物件中，並將結果儲存在這個新的多項式中，同時使用add函式的功能
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result(capacity + other.capacity);
        for (int i = 0; i < terms; ++i) {
            result.add(termArray[i].coef, termArray[i].exp);
        }
        for (int i = 0; i < other.terms; ++i) {
            result.add(other.termArray[i].coef, other.termArray[i].exp);
        }
        return result;
    }
    //與上面的程式大致上同理，但是乘法的版本。 
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result(capacity * other.capacity);
        for (int i = 0; i < terms; ++i) {
            for (int j = 0; j < other.terms; ++j) {
                float newCoef = termArray[i].coef * other.termArray[j].coef;
                int newExp = termArray[i].exp + other.termArray[j].exp;
                result.add(newCoef, newExp);
            }
        }
        return result;
    }
    //用來從用戶輸入多項式，並將其添加到Polynomial物件中。
    friend istream& operator>>(istream& is, Polynomial& poly) {
        int numTerms;
        cout << "請輸入多項式數量: ";
        is >> numTerms;
        for (int i = 0; i < numTerms; ++i) {
            float coef;
            int exp;
            cout << "請依序輸入係數及次方項: ";
            is >> coef >> exp;
            poly.add(coef, exp);
        }
        return is;
    }
    //用來將 Polynomial 物件的內容輸出。
    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        for (int i = 0; i < poly.terms; ++i) {
            if (i > 0 && poly.termArray[i].coef > 0)
                os << " + ";
            os << poly.termArray[i].coef;
            if (poly.termArray[i].exp != 0) {
                os << "x^" << poly.termArray[i].exp;
            }
        }
        return os;
    }

private:
    Term* termArray;
    int capacity;
    int terms;
    //用來擴展陣列，當termArray陣列的容量不足時，將原本的內容複製到新的且較大的新空間裡，並刪除舊的空間。
    void resize() {
        capacity *= 2;
        Term* newArray = new Term[capacity];
        for (int i = 0; i < terms; ++i) {
            newArray[i] = termArray[i];
        }
        delete[] termArray;
        termArray = newArray;
    }
    //它會將所有後續的項向前移動一個位置，填補被刪除項的位置。
    void removeTerm(int index) {
        for (int i = index; i < terms - 1; ++i) {
            termArray[i] = termArray[i + 1];
        }
        --terms;
    }
    //根據exp來進行排序，使多項式的次方以高排到低。
    void sortTerms() {
        for (int i = 0; i < terms - 1; ++i) {
            for (int j = i + 1; j < terms; ++j) {
                if (termArray[i].exp < termArray[j].exp) {
                    swap(termArray[i], termArray[j]);
                }
            }
        }
    }
};

int main() {
    Polynomial p1, p2;
    cout << "請輸入第一個多項式:\n";
    cin >> p1;
    cout << "請輸入第二個多項式:\n";
    cin >> p2;
    //輸入多項式
    cout << "\n第一個多項式為: " << p1 << endl;
    cout << "第二個多項式為: " << p2 << endl;
    //輸出多項式
    Polynomial sum = p1 + p2;
    Polynomial product = p1 * p2;
    //計算多項式的相加與相乘
    cout << "\n合: " << sum << endl;
    cout << "積: " << product << endl;
    //將值帶入多項式中計算
    float x;
    cout << "\n請輸入多項式要計算之值:" << endl;
    cout << "x為: ";
    cin >> x;
    cout << "\nP1(" << x << ") = " << p1.evaluate(x) << endl;
    cout << "P2(" << x << ") = " << p2.evaluate(x) << endl;
    cout << "(P1 + P2)(" << x << ") = " << sum.evaluate(x) << endl;
    cout << "(P1 * P2)(" << x << ") = " << product.evaluate(x) << endl;

    return 0;
}
