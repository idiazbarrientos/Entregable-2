#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <unordered_map>
#include <random>
#include <numeric>

using namespace std;
/*Tabla con hashing abierto(encadenamiento)*/

// Función hash para User ID
int hash_function_user_id(int user_id, int table_size) {
    return user_id % table_size;
}

// Función hash para User Name
int hash_function_user_name(const string& user_name, int table_size) {
    int hash_value = 0;
    for (char c : user_name) {
        hash_value = (hash_value * 31 + c) % table_size;
    }
    return hash_value;
}

class HashTableOpen {
public:
    int size;
    vector<list<pair<int, string> > > table; // Lista de pares (User ID, User Name)

    HashTableOpen(int size) : size(size) {
        table.resize(size);
    }

    void insert(int user_id, const string& user_name) {
        int index = hash_function_user_id(user_id, size);
        table[index].emplace_back(user_id, user_name);
    }

    bool search(int user_id, const string& user_name) {
        int index = hash_function_user_id(user_id, size);
        for (const auto& element : table[index]) {
            if (element.first == user_id && element.second == user_name) {
                return true;
            }
        }
        return false;
    }
};
/*Tabla con hashing cerrado*/

const float A = (sqrt(5) - 1) / 2;

// Método de la división
int h1(int k, int n) {
    return k % n;
}

// Método de la multiplicación
int h2(int k, int n) {
    float a = (float)k * A;
    a -= (int)a; // Esta línea implementa la operación módulo 1 (%1)
    return n * a;
}

class HashTableClosed {
public:
    int size;
    vector<pair<int, string> > table; // Par (User ID, User Name)
    vector<bool> occupied;

    HashTableClosed(int size) : size(size) {
        table.resize(size, {-1, ""});
        occupied.resize(size, false);
    }

    int linear_probing(int key, int i) {
        return (h1(key, size) + i) % size;
    }

    int quadratic_probing(int key, int i) {
        return (h1(key, size) + i + 2 * i * i) % size;
    }

    int double_hashing(int key, int i) {
        return (h1(key, size) + i * (h2(key, size) + 1)) % size;
    }

    void insert(int key, const string& user_name, int (HashTableClosed::*probe_function)(int, int)) {
        int i = 0;
        int index;
        do {
            index = (this->*probe_function)(key, i);
            if (!occupied[index]) {
                table[index] = {key, user_name};
                occupied[index] = true;
                return;
            }
            i++;
        } while (i < size);
    }

    bool search(int key, const string& user_name, int (HashTableClosed::*probe_function)(int, int)) {
        int i = 0;
        int index;
        do {
            index = (this->*probe_function)(key, i);
            if (!occupied[index]) {
                return false;
            }
            if (table[index].first == key && table[index].second == user_name) {
                return true;
            }
            i++;
        } while (i < size);
        return false;
    }
};
/*pruebas de rendimiento*/
/*hashing abierto*/
void test_insert(HashTableOpen &ht, const vector<pair<int, string> >& data, const char* out_file) {
    ofstream file_out(out_file);
    auto start = chrono::high_resolution_clock::now();
    for (const auto& entry : data) {
        ht.insert(entry.first, entry.second);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    file_out << "Insert," << duration.count() << endl;
    file_out.close();
}

void test_search(HashTableOpen &ht, const vector<pair<int, string> >& data, const char* out_file) {
    ofstream file_out(out_file);
    auto start = chrono::high_resolution_clock::now();
    for (const auto& entry : data) {
        ht.search(entry.first, entry.second);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    file_out << "Search," << duration.count() << endl;
    file_out.close();
}
/*pruebas de rendimiento*/
/*hashing cerrado*/
void test_insert(HashTableClosed &ht, const vector<pair<int, string> >& data, const char* out_file, int (HashTableClosed::*probe_function)(int, int)) {
    ofstream file_out(out_file);
    auto start = chrono::high_resolution_clock::now();
    for (const auto& entry : data) {
        ht.insert(entry.first, entry.second, probe_function);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    file_out << "Insert," << duration.count() << endl;
    file_out.close();
}

void test_search(HashTableClosed &ht, const vector<pair<int, string> >& data, const char* out_file, int (HashTableClosed::*probe_function)(int, int)) {
    ofstream file_out(out_file);
    auto start = chrono::high_resolution_clock::now();
    for (const auto& entry : data) {
        ht.search(entry.first, entry.second, probe_function);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    file_out << "Search," << duration.count() << endl;
    file_out.close();
}
/*pruebas de rendimiento*/
/*unorderer map*/
void test_insert(unordered_map<int, string> &um, const vector<pair<int, string> >& data, const char* out_file) {
    ofstream file_out(out_file);
    auto start = chrono::high_resolution_clock::now();
    for (const auto& entry : data) {
        um[entry.first] = entry.second;
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    file_out << "Insert," << duration.count() << endl;
    file_out.close();
}

void test_search(unordered_map<int, string> &um, const vector<pair<int, string> >& data, const char* out_file) {
    ofstream file_out(out_file);
    auto start = chrono::high_resolution_clock::now();
    for (const auto& entry : data) {
        um.find(entry.first);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    file_out << "Search," << duration.count() << endl;
    file_out.close();
}
/*lectura del data set*/
vector<pair<int, string> > read_data(const char* filename) {
    vector<pair<int, string> > data;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string university, user_id_str, user_name, num_tweets, friends_count, followers_count, created_at;
        getline(ss, university, ',');
        getline(ss, user_id_str, ',');
        getline(ss, user_name, ',');
        getline(ss, num_tweets, ',');
        getline(ss, friends_count, ',');
        getline(ss, followers_count, ',');
        getline(ss, created_at, ',');
        int user_id = stoi(user_id_str);
        data.emplace_back(user_id, user_name);
    }
    file.close();
    return data;
}