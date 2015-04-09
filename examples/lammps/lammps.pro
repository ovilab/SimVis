TEMPLATE = app
CONFIG += c++11

QT += qml quick widgets opengl openglextensions
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-unused-variable
CONFIG += warn_off

SOURCES += \
    main.cpp \
    mysimulator.cpp \
    angle_hybrid.cpp \
    angle.cpp \
    atom_map.cpp \
    atom_vec_atomic.cpp \
    atom_vec_body.cpp \
    atom_vec_charge.cpp \
    atom_vec_ellipsoid.cpp \
    atom_vec_hybrid.cpp \
    atom_vec_line.cpp \
    atom_vec_sphere.cpp \
    atom_vec_tri.cpp \
    atom_vec.cpp \
    atom.cpp \
    balance.cpp \
    body.cpp \
    bond_hybrid.cpp \
    bond.cpp \
    change_box.cpp \
    citeme.cpp \
    comm_brick.cpp \
    comm_tiled.cpp \
    comm.cpp \
    compute_angle_local.cpp \
    compute_atom_molecule.cpp \
    compute_bond_local.cpp \
    compute_centro_atom.cpp \
    compute_cluster_atom.cpp \
    compute_cna_atom.cpp \
    compute_com_molecule.cpp \
    compute_com.cpp \
    compute_contact_atom.cpp \
    compute_coord_atom.cpp \
    compute_dihedral_local.cpp \
    compute_displace_atom.cpp \
    compute_erotate_sphere_atom.cpp \
    compute_erotate_sphere.cpp \
    compute_group_group.cpp \
    compute_gyration_molecule.cpp \
    compute_gyration.cpp \
    compute_heat_flux.cpp \
    compute_improper_local.cpp \
    compute_inertia_molecule.cpp \
    compute_ke_atom.cpp \
    compute_ke.cpp \
    compute_msd_molecule.cpp \
    compute_msd.cpp \
    compute_pair_local.cpp \
    compute_pair.cpp \
    compute_pe_atom.cpp \
    compute_pe.cpp \
    compute_pressure.cpp \
    compute_property_atom.cpp \
    compute_property_local.cpp \
    compute_property_molecule.cpp \
    compute_rdf.cpp \
    compute_reduce_region.cpp \
    compute_reduce.cpp \
    compute_slice.cpp \
    compute_stress_atom.cpp \
    compute_temp_com.cpp \
    compute_temp_deform.cpp \
    compute_temp_partial.cpp \
    compute_temp_profile.cpp \
    compute_temp_ramp.cpp \
    compute_temp_region.cpp \
    compute_temp_sphere.cpp \
    compute_temp.cpp \
    compute_vacf.cpp \
    compute_vcm_molecule.cpp \
    compute.cpp \
    create_atoms.cpp \
    create_box.cpp \
    delete_atoms.cpp \
    delete_bonds.cpp \
    dihedral_hybrid.cpp \
    dihedral.cpp \
    displace_atoms.cpp \
    domain.cpp \
    dump_atom.cpp \
    dump_cfg.cpp \
    dump_custom.cpp \
    dump_dcd.cpp \
    dump_image.cpp \
    dump_local.cpp \
    dump_movie.cpp \
    dump_xyz.cpp \
    dump.cpp \
    error.cpp \
    finish.cpp \
    fix_adapt.cpp \
    fix_addforce.cpp \
    fix_ave_atom.cpp \
    fix_ave_correlate.cpp \
    fix_ave_histo.cpp \
    fix_ave_spatial.cpp \
    fix_ave_time.cpp \
    fix_aveforce.cpp \
    fix_balance.cpp \
    fix_box_relax.cpp \
    fix_deform.cpp \
    fix_drag.cpp \
    fix_dt_reset.cpp \
    fix_enforce2d.cpp \
    fix_external.cpp \
    fix_gravity.cpp \
    fix_group.cpp \
    fix_heat.cpp \
    fix_indent.cpp \
    fix_langevin.cpp \
    fix_lineforce.cpp \
    fix_minimize.cpp \
    fix_momentum.cpp \
    fix_move.cpp \
    fix_nh_sphere.cpp \
    fix_nh.cpp \
    fix_nph_sphere.cpp \
    fix_nph.cpp \
    fix_npt_sphere.cpp \
    fix_npt.cpp \
    fix_nve_limit.cpp \
    fix_nve_noforce.cpp \
    fix_nve_sphere.cpp \
    fix_nve.cpp \
    fix_nvt_sllod.cpp \
    fix_nvt_sphere.cpp \
    fix_nvt.cpp \
    fix_planeforce.cpp \
    fix_press_berendsen.cpp \
    fix_print.cpp \
    fix_property_atom.cpp \
    fix_read_restart.cpp \
    fix_recenter.cpp \
    fix_respa.cpp \
    fix_restrain.cpp \
    fix_setforce.cpp \
    fix_shear_history.cpp \
    fix_spring_rg.cpp \
    fix_spring_self.cpp \
    fix_spring.cpp \
    fix_store_force.cpp \
    fix_store_state.cpp \
    fix_store.cpp \
    fix_temp_berendsen.cpp \
    fix_temp_csvr.cpp \
    fix_temp_rescale.cpp \
    fix_tmd.cpp \
    fix_vector.cpp \
    fix_viscous.cpp \
    fix_wall_harmonic.cpp \
    fix_wall_lj93.cpp \
    fix_wall_lj126.cpp \
    fix_wall_lj1043.cpp \
    fix_wall_reflect.cpp \
    fix_wall_region.cpp \
    fix_wall.cpp \
    fix.cpp \
    force.cpp \
    group.cpp \
    image.cpp \
    improper_hybrid.cpp \
    improper.cpp \
    input.cpp \
    integrate.cpp \
    irregular.cpp \
    kspace.cpp \
    lammps.cpp \
    lattice.cpp \
    library.cpp \
    math_extra.cpp \
    memory.cpp \
    min_cg.cpp \
    min_fire.cpp \
    min_hftn.cpp \
    min_linesearch.cpp \
    min_quickmin.cpp \
    min_sd.cpp \
    min.cpp \
    minimize.cpp \
    modify.cpp \
    molecule.cpp \
    neigh_bond.cpp \
    neigh_derive.cpp \
    neigh_full.cpp \
    neigh_gran.cpp \
    neigh_half_bin.cpp \
    neigh_half_multi.cpp \
    neigh_half_nsq.cpp \
    neigh_list.cpp \
    neigh_request.cpp \
    neigh_respa.cpp \
    neigh_stencil.cpp \
    neighbor.cpp \
    output.cpp \
    pair_beck.cpp \
    pair_born_coul_wolf.cpp \
    pair_born.cpp \
    pair_buck_coul_cut.cpp \
    pair_buck.cpp \
    pair_coul_cut.cpp \
    pair_coul_debye.cpp \
    pair_coul_dsf.cpp \
    pair_coul_wolf.cpp \
    pair_dpd_tstat.cpp \
    pair_dpd.cpp \
    pair_gauss.cpp \
    pair_hybrid_overlay.cpp \
    pair_hybrid.cpp \
    pair_lj_cubic.cpp \
    pair_lj_cut_coul_cut.cpp \
    pair_lj_cut_coul_debye.cpp \
    pair_lj_cut_coul_dsf.cpp \
    pair_lj_cut.cpp \
    pair_lj_expand.cpp \
    pair_lj_gromacs_coul_gromacs.cpp \
    pair_lj_gromacs.cpp \
    pair_lj_smooth_linear.cpp \
    pair_lj_smooth.cpp \
    pair_lj96_cut.cpp \
    pair_mie_cut.cpp \
    pair_morse.cpp \
    pair_soft.cpp \
    pair_table.cpp \
    pair_yukawa.cpp \
    pair_zbl.cpp \
    pair.cpp \
    procmap.cpp \
    random_mars.cpp \
    random_park.cpp \
    rcb.cpp \
    read_data.cpp \
    read_dump.cpp \
    read_restart.cpp \
    reader_native.cpp \
    reader_xyz.cpp \
    reader.cpp \
    region_block.cpp \
    region_cone.cpp \
    region_cylinder.cpp \
    region_intersect.cpp \
    region_plane.cpp \
    region_prism.cpp \
    region_sphere.cpp \
    region_union.cpp \
    region.cpp \
    replicate.cpp \
    rerun.cpp \
    respa.cpp \
    run.cpp \
    set.cpp \
    special.cpp \
    thermo.cpp \
    timer.cpp \
    universe.cpp \
    update.cpp \
    variable.cpp \
    velocity.cpp \
    verlet.cpp \
    write_data.cpp \
    write_dump.cpp \
    write_restart.cpp \
    mpi.c

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(../../library_deployment.pri)

HEADERS += \
    mysimulator.h \
    accelerator_cuda.h \
    accelerator_intel.h \
    accelerator_kokkos.h \
    accelerator_omp.h \
    angle_hybrid.h \
    angle.h \
    atom_masks.h \
    atom_vec_atomic.h \
    atom_vec_body.h \
    atom_vec_charge.h \
    atom_vec_ellipsoid.h \
    atom_vec_hybrid.h \
    atom_vec_line.h \
    atom_vec_sphere.h \
    atom_vec_tri.h \
    atom_vec.h \
    atom.h \
    balance.h \
    body.h \
    bond_hybrid.h \
    bond.h \
    change_box.h \
    citeme.h \
    comm_brick.h \
    comm_tiled.h \
    comm.h \
    compute_angle_local.h \
    compute_atom_molecule.h \
    compute_bond_local.h \
    compute_centro_atom.h \
    compute_cluster_atom.h \
    compute_cna_atom.h \
    compute_com_molecule.h \
    compute_com.h \
    compute_contact_atom.h \
    compute_coord_atom.h \
    compute_dihedral_local.h \
    compute_displace_atom.h \
    compute_erotate_sphere_atom.h \
    compute_erotate_sphere.h \
    compute_group_group.h \
    compute_gyration_molecule.h \
    compute_gyration.h \
    compute_heat_flux.h \
    compute_improper_local.h \
    compute_inertia_molecule.h \
    compute_ke_atom.h \
    compute_ke.h \
    compute_msd_molecule.h \
    compute_msd.h \
    compute_pair_local.h \
    compute_pair.h \
    compute_pe_atom.h \
    compute_pe.h \
    compute_pressure.h \
    compute_property_atom.h \
    compute_property_local.h \
    compute_property_molecule.h \
    compute_rdf.h \
    compute_reduce_region.h \
    compute_reduce.h \
    compute_slice.h \
    compute_stress_atom.h \
    compute_temp_com.h \
    compute_temp_deform.h \
    compute_temp_partial.h \
    compute_temp_profile.h \
    compute_temp_ramp.h \
    compute_temp_region.h \
    compute_temp_sphere.h \
    compute_temp.h \
    compute_vacf.h \
    compute_vcm_molecule.h \
    compute.h \
    create_atoms.h \
    create_box.h \
    delete_atoms.h \
    delete_bonds.h \
    dihedral_hybrid.h \
    dihedral.h \
    displace_atoms.h \
    domain.h \
    dump_atom.h \
    dump_cfg.h \
    dump_custom.h \
    dump_dcd.h \
    dump_image.h \
    dump_local.h \
    dump_movie.h \
    dump_xyz.h \
    dump.h \
    error.h \
    finish.h \
    fix_adapt.h \
    fix_addforce.h \
    fix_ave_atom.h \
    fix_ave_correlate.h \
    fix_ave_histo.h \
    fix_ave_spatial.h \
    fix_ave_time.h \
    fix_aveforce.h \
    fix_balance.h \
    fix_box_relax.h \
    fix_deform.h \
    fix_drag.h \
    fix_dt_reset.h \
    fix_enforce2d.h \
    fix_external.h \
    fix_gravity.h \
    fix_group.h \
    fix_heat.h \
    fix_indent.h \
    fix_langevin.h \
    fix_lineforce.h \
    fix_minimize.h \
    fix_momentum.h \
    fix_move.h \
    fix_nh_sphere.h \
    fix_nh.h \
    fix_nph_sphere.h \
    fix_nph.h \
    fix_npt_sphere.h \
    fix_npt.h \
    fix_nve_limit.h \
    fix_nve_noforce.h \
    fix_nve_sphere.h \
    fix_nve.h \
    fix_nvt_sllod.h \
    fix_nvt_sphere.h \
    fix_nvt.h \
    fix_planeforce.h \
    fix_press_berendsen.h \
    fix_print.h \
    fix_property_atom.h \
    fix_read_restart.h \
    fix_recenter.h \
    fix_respa.h \
    fix_restrain.h \
    fix_setforce.h \
    fix_shear_history.h \
    fix_spring_rg.h \
    fix_spring_self.h \
    fix_spring.h \
    fix_store_force.h \
    fix_store_state.h \
    fix_store.h \
    fix_temp_berendsen.h \
    fix_temp_csvr.h \
    fix_temp_rescale.h \
    fix_tmd.h \
    fix_vector.h \
    fix_viscous.h \
    fix_wall_harmonic.h \
    fix_wall_lj93.h \
    fix_wall_lj126.h \
    fix_wall_lj1043.h \
    fix_wall_reflect.h \
    fix_wall_region.h \
    fix_wall.h \
    fix.h \
    force.h \
    group.h \
    image.h \
    improper_hybrid.h \
    improper.h \
    input.h \
    integrate.h \
    irregular.h \
    kspace.h \
    lammps.h \
    lattice.h \
    library.h \
    lmptype.h \
    math_const.h \
    math_extra.h \
    math_special.h \
    memory.h \
    min_cg.h \
    min_fire.h \
    min_hftn.h \
    min_linesearch.h \
    min_quickmin.h \
    min_sd.h \
    min.h \
    minimize.h \
    modify.h \
    molecule.h \
    mpi.h \
    mpiio.h \
    my_page.h \
    my_pool_chunk.h \
    neigh_bond.h \
    neigh_derive.h \
    neigh_full.h \
    neigh_gran.h \
    neigh_half_bin.h \
    neigh_half_multi.h \
    neigh_half_nsq.h \
    neigh_list.h \
    neigh_request.h \
    neigh_respa.h \
    neighbor.h \
    output.h \
    pair_beck.h \
    pair_born_coul_wolf.h \
    pair_born.h \
    pair_buck_coul_cut.h \
    pair_buck.h \
    pair_coul_cut.h \
    pair_coul_debye.h \
    pair_coul_dsf.h \
    pair_coul_wolf.h \
    pair_dpd_tstat.h \
    pair_dpd.h \
    pair_gauss.h \
    pair_hybrid_overlay.h \
    pair_hybrid.h \
    pair_lj_cubic.h \
    pair_lj_cut_coul_cut.h \
    pair_lj_cut_coul_debye.h \
    pair_lj_cut_coul_dsf.h \
    pair_lj_cut.h \
    pair_lj_expand.h \
    pair_lj_gromacs_coul_gromacs.h \
    pair_lj_gromacs.h \
    pair_lj_smooth_linear.h \
    pair_lj_smooth.h \
    pair_lj96_cut.h \
    pair_mie_cut.h \
    pair_morse.h \
    pair_soft.h \
    pair_table.h \
    pair_yukawa.h \
    pair_zbl.h \
    pair.h \
    pointers.h \
    procmap.h \
    random_mars.h \
    random_park.h \
    rcb.h \
    read_data.h \
    read_dump.h \
    read_restart.h \
    reader_native.h \
    reader_xyz.h \
    reader.h \
    region_block.h \
    region_cone.h \
    region_cylinder.h \
    region_intersect.h \
    region_plane.h \
    region_prism.h \
    region_sphere.h \
    region_union.h \
    region.h \
    replicate.h \
    rerun.h \
    respa.h \
    run.h \
    set.h \
    special.h \
    style_angle.h \
    style_atom.h \
    style_body.h \
    style_bond.h \
    style_command.h \
    style_compute.h \
    style_dihedral.h \
    style_dump.h \
    style_fix.h \
    style_improper.h \
    style_integrate.h \
    style_kspace.h \
    style_minimize.h \
    style_pair.h \
    style_reader.h \
    style_region.h \
    suffix.h \
    thermo.h \
    timer.h \
    universe.h \
    update.h \
    variable.h \
    velocity.h \
    verlet.h \
    version.h \
    write_data.h \
    write_dump.h \
    write_restart.h

DISTFILES +=
