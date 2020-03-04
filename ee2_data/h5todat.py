import h5py
import numpy as np

fp = h5py.File("EE2_cut1_zStpStart60.hdf5", 'r')

# principal components
PCs = np.zeros((15,61913))
PCs[0,:] = np.array(fp['PCA']['mean']).reshape(613*101)
for i in range(1,15):
    PCs[i,:] = np.array(fp['PCA']['PC'+str(i)]).reshape(613*101)

np.savetxt("ee2_pcs.dat", PCs,delimiter=",")

# PCE coefficients
with open("pce_coeffs.dat", "w") as outfile:
    for i in range(1,15):
        coeff = np.array(fp['PCE']['PC'+str(i)]["Coefficients"])
        outfile.write(','.join(map(str,coeff))+"\n")

outfile.close()

# PCE multi-indices 
with open("pce_multiindices.dat", "w") as multiindex_file:
    for i in range(1,15):
        multiindices = np.array(fp['PCE']['PC'+str(i)]["Indices"]).T
        multiindices = multiindices.reshape(8*multiindices.shape[0])
        multiindex_file.write(','.join(map(str,multiindices))+"\n")

multiindex_file.close()
