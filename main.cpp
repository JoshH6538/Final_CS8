#include <iostream>
#include <iomanip>
#include "includes/sql/sql.h"


using namespace std;

int main(int argv, char** argc) {
    cout << "\n\n"
         << endl;
    // SQL sql;
    // sql.welcome();
    // sql.runtime();
    cout << "\n\n\n=====================" << endl;
    vector<int> indices;
    indices.emplace_back();
    vector<int> nums={2,7,11,15};
    cout<<"nums: "<<nums.size()<<endl;
    int target=9;
    for(int i=0;i<nums.size();i++)
    {
        int j=i+1;
        while(j<nums.size())
        {
            if((nums[i]+nums[j])==target)
            {
                bool exist_i=false, exist_j=false;
                for(int k=0;k<indices.size();k++)
                {
                    if(indices[k]==i) exist_i=true;
                    if(indices[k]==j) exist_j=true;
                }
                // cout<<nums[i]<<" + "<<nums[j]<<" = "<<target<<endl;
                // cout<<"inserting "<<i<<endl;
                if(!exist_i)indices.push_back(i);
                // cout<<"inserting "<<j<<endl;
                if(!exist_j)indices.push_back(j);
            }
            j++;
        }
    }
    cout<<"size: "<<indices.size()<<endl;
    for(int i=0;i<indices.size();i++)
    {
        cout<<indices[i]<<endl;
    }
    return 0;
}
