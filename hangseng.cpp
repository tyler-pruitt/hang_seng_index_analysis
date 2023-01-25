#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
 
using namespace std;

vector<vector<string>> get_csv_data(string fname);
void display_csv_data(vector<vector<string>> content);

class IndexData {
    public:
        vector<double> price;
        vector<string> date;
        vector<int> year, month, day;

        vector<double> daily_change;
        vector<double> year_over_year_change;
        int first_year, last_year;
        double average_yearly_return;

        IndexData(vector<vector<string>> data) {

            first_year = stoi(data[16][0].substr(0, 4));
            last_year = stoi(data[data.size() - 1][0].substr(0, 4));

            int previous_year = first_year;
            double previous_year_start_price = stod(data[16][1]);

            for (int i=16;i<data.size();i++) {
                price.push_back(stod(data[i][1]));
                date.push_back(data[i][0]);

                year.push_back(stoi(data[i][0].substr(0, 4)));
                month.push_back(stoi(data[i][0].substr(5, 2)));
                day.push_back(stoi(data[i][0].substr(8, 2)));

                if (i > 16) {
                    double difference = ((stod(data[i][1]) - stod(data[i-1][1])) * 100.0) / stod(data[i-1][1]);

                    daily_change.push_back(difference);
                } else {
                    daily_change.push_back(0.00);
                }

                if (stoi(data[i][0].substr(0, 4)) != previous_year) {
                    double yoy_change = ((stod(data[i][1]) - previous_year_start_price) * 100.0) / previous_year_start_price;

                    year_over_year_change.push_back(yoy_change);

                    // Update previous year and previous year starting price
                    previous_year++;
                    previous_year_start_price = stod(data[i][1]);
                }
            }

            average_yearly_return = 0.00;

            for (int j=0;j<year_over_year_change.size();j++) {
                average_yearly_return += year_over_year_change[j];
            }

            average_yearly_return /= (last_year - first_year);
        }

        void print() {
            cout << "date, price, daily_change" << endl;

            for (int i=0;i<price.size();i++) {
                cout << date[i] << ", $" << price[i] << ", " << daily_change[i] << "%" << endl;
            }
        }
};

int main() {
    vector<vector<string>> hang_seng_index_csv_data = get_csv_data("hang-seng-composite-index-historical-data.csv");

    //display_csv_data(hang_seng_index_csv_data);

    IndexData hang_seng_index_data = IndexData(hang_seng_index_csv_data);

    hang_seng_index_data.print();

    cout << "first year: " << hang_seng_index_data.first_year << endl;
    cout << "last year: " << hang_seng_index_data.last_year << endl;

    cout << "yoy change:" << endl;
    for (int i=0;i<hang_seng_index_data.year_over_year_change.size();i++) {
        cout << hang_seng_index_data.first_year + i << ": " << hang_seng_index_data.year_over_year_change[i] << "%" << endl;
    }

    cout << "average yearly return: " << hang_seng_index_data.average_yearly_return << "%" << endl;

    return 0;
}

vector<vector<string>> get_csv_data(string fname) {
    vector<vector<string>> content;
    vector<string> row;
    string line, word;
    
    fstream file (fname, ios::in);

    if (file.is_open()) {
        while(getline(file, line)) {
            row.clear();
            
            stringstream str(line);
            
            while(getline(str, word, ',')) {
                row.push_back(word);
            }

            content.push_back(row);
        }
    } else {
        cout << "Could not open the file\n";
        
        for(int i=0;i<content.size();i++) {
            for(int j=0;j<content[i].size();j++) {
                cout<<content[i][j]<<" ";
            }

            cout<<"\n";
        }
    }

    return content;
}

void display_csv_data(vector<vector<string>> content) {
    for (int i=0;i<content.size();i++) {
        for (int j=0;j<content[i].size();j++) {
            if (j != content[i].size() - 1) {
                cout << content[i][j] << ", ";
            } else {
                cout << content[i][j];
            }
        }

        cout << endl;
    }
}