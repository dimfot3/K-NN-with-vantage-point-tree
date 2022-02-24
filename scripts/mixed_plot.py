import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

plt.style.use(['science'])

df_1_mode_1 = pd.read_csv('mixed_test/dt1_mode_1.csv')
df_2_mode_1 = pd.read_csv('mixed_test/dt2_mode_1.csv')
df_3_mode_1 = pd.read_csv('mixed_test/dt3_mode_1.csv')
df_1_mode_2 = pd.read_csv('mixed_test/dt1_mode_2.csv')
df_2_mode_2 = pd.read_csv('mixed_test/dt2_mode_2.csv')
df_3_mode_2 = pd.read_csv('mixed_test/dt3_mode_2.csv')

##serial vs parallel increasing number of points
datasets = [df_1_mode_1, df_2_mode_1, df_3_mode_1, df_1_mode_2, df_2_mode_2, df_3_mode_2]
labels = ['Simple OpenMP(10000x3000)', 'Simple OpenMP(30000x10000)', 'Simple OpenMP(55000x3000)', 'Mixed CPU/OpenMP(10000x3000)',
          'Mixed CPU/OpenMP(30000x10000)','Mixed CPU/OpenMP(55000x3000)']
for i in range(len(datasets)):
    ### serial parallel compare simple plot
    n_vals = df_2_mode_1.max_threads.unique()
    times = np.array([], dtype=float)
    for j in range(n_vals.shape[0]):
        times = np.append(times, np.mean(datasets[i].loc[datasets[i].max_threads == n_vals[j], 'vp_creation_time']))
    plt.plot(n_vals, times, label=labels[i])

plt.title('Mixed Limited version vs Simple OpenMP', fontsize=20)
plt.legend(fontsize=18)
plt.xlim([1,8])
plt.xlabel('maximum live threads', fontsize=18)
plt.ylabel('Execution time(ms)', fontsize=18)
plt.show()



"""
##serial vs parallel increasing number of points
datasets = [df_ser_uni_n, df_ser_norm_n, df_ser_exp_n, df_par_uni_n, df_par_norm_n, df_par_exp_n]
labels = ['uniform serial', 'normal serial', 'expodential serial', 'uniform OpenMP', 'normal OpenMP','expodential OpenMP']
for i in range(len(datasets)):
    ### serial parallel compare simple plot
    n_vals = df_par_uni_n.num.unique()
    times = np.array([], dtype=float)
    for j in range(n_vals.shape[0]):
        times = np.append(times, np.mean(datasets[i].loc[datasets[i].num == n_vals[j], 'vp_creation_time']))
    plt.plot(n_vals, times, label=labels[i])

plt.title('Serial vs Parallel(18 maximum threads) with increasing number of points(dim=2000)', fontsize=20)
plt.legend(fontsize=18)
plt.xlabel('Number of points', fontsize=18)
plt.ylabel('Execution time(ms)', fontsize=18)
plt.show()


##serial vs parallel increasing number of dimension
datasets = [df_ser_uni_d, df_ser_norm_d, df_ser_exp_d, df_par_uni_d, df_par_norm_d, df_par_exp_d]
labels = ['uniform serial', 'normal serial', 'expodential serial', 'uniform OpenMP', 'normal OpenMP','expodential OpenMP']
for i in range(len(datasets)):
    ### serial parallel compare simple plot
    n_vals = df_par_uni_d.dim.unique()
    times = np.array([], dtype=float)
    for j in range(n_vals.shape[0]):
        times = np.append(times, np.mean(datasets[i].loc[datasets[i].dim == n_vals[j], 'vp_creation_time']))
    plt.plot(n_vals, times, label=labels[i])

plt.title('Serial vs Parallel(18 maximum threads) with increasing number of dimension(num=20000)', fontsize=20)
plt.legend(fontsize=18)
plt.xlabel('Number of dimension', fontsize=18)
plt.ylabel('Execution time(ms)', fontsize=18)
plt.show()

def keep_unique(prod, times):
    new_prod = np.array([], dtype=float)
    new_times = np.array([], dtype=float)
    un_prod = np.unique(prod)
    for i in range(un_prod.shape[0]):
        idxs = np.argwhere(prod == un_prod[i])
        new_prod = np.append(new_prod, un_prod[i])
        new_times = np.append(new_times, np.mean(times[idxs]))
    return new_prod, new_times


##serial vs parallel increasing product
dt1 = [df_ser_uni_n, df_ser_norm_n, df_ser_exp_n, df_ser_uni_d, df_ser_norm_d, df_ser_exp_d]
dt2 = [df_par_uni_n, df_par_norm_n, df_par_exp_n, df_par_uni_d, df_par_norm_d, df_par_exp_d]

labels = ['Serial', 'OpenMP']
### serial parallel compare simple plot
#serial
prods = np.array([], dtype=float)
times = np.array([], dtype=float)
for j in range(len(dt1)):
    prods = np.append(prods, dt1[j].num * dt1[j].dim)
    times = np.append(times, dt1[j].vp_creation_time)
idxs = prods.argsort()
prods = prods[idxs]
times = times[idxs]
prods, times = keep_unique(prods, times)
plt.plot(prods, times, label='Serial')
sprods = prods.copy()
stimes = times.copy()
#OpenMP
prods = np.array([], dtype=float)
times = np.array([], dtype=float)
for j in range(len(dt2)):
    prods = np.append(prods, dt2[j].num * dt2[j].dim)
    times = np.append(times, dt2[j].vp_creation_time)
idxs = prods.argsort()
prods = prods[idxs]
times = times[idxs]
prods, times = keep_unique(prods, times)

plt.plot(prods, times, label='OpenMP')


plt.title('Serial vs Parallel(18 maximum threads) with increasing product num x dim', fontsize=20)
plt.legend(fontsize=18)
plt.xlabel('Number of product', fontsize=18)
plt.ylabel('Execution time(ms)', fontsize=18)
sub_axes = plt.axes([.3, .6, .25, .25])
sub_axes.plot(sprods, stimes, prods, times)
sub_axes.set_title('Zoom in')
sub_axes.set_xlim([0,200000])
sub_axes.set_ylim([0,30])

plt.show()
"""



