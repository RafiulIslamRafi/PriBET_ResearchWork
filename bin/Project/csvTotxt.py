print("CSV to txt file Convert\t: ", end=' ')
import pandas as pd 
df = pd.read_csv('Project/COVIDandFLUdata.csv',header= 0,index_col= False)
df.columns = df.columns.str.strip().str.replace(' ', '') # Column Name er Space Remover

#df Convert:
for col in df.columns: # all Value er Space Remover
    try:
        df[col] = df[col].str.replace(' ', '')
    except AttributeError:
        pass
df = df.fillna(-1) # faka value -1 diye set
df['Age'] = df['Age'].astype(int) # integer e convert

import os
base_filename = 'csv_data.txt'
with open(os.path.join('Project', base_filename),'w') as outfile:
    df.to_string(outfile)
print("Done")
