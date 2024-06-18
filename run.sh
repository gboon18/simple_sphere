for i in {1..5}
do
    #time as the random seed
    ./gdml_det sphere_hdpe.gdml run.mac sphere_hdpe_1e5_$i.csv
    ./gdml_det sphere_stainlesssteel.gdml run.mac sphere_stainlesssteel_1e5_$i.csv
done
