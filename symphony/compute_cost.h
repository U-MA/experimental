/*===========================================================================*/
/*                                                                           */
/* This file is part of a demonstration application for use with the         */
/* SYMPHONY Branch, Cut, and Price Library. This application is a solver for */
/* the Vehicle Routing Problem and the Traveling Salesman Problem.           */
/*                                                                           */
/* (c) Copyright 2000-2011 Ted Ralphs. All Rights Reserved.                  */
/*                                                                           */
/* This application was developed by Ted Ralphs (ted@lehigh.edu)             */
/*                                                                           */
/* This software is licensed under the Eclipse Public License. Please see    */
/* accompanying file for terms.                                              */
/*                                                                           */
/*===========================================================================*/

#ifndef _COMPUTE_COST_H
#define _COMPUTE_COST_H

#include "sym_proto.h"
#include "vrp_common_types.h"

int compute_icost PROTO((distances *dist, int v0, int v1));
void canonical_tour PROTO((distances *dist, best_tours *cur_tour,
			   int vertnum, int capacity, int *demand));
int route_calc PROTO((distances *dist, _node *tour, int numroutes, 
		      route_data *route_info, int *demand));
int compute_tour_cost PROTO((distances *dist, _node *tour));
double ECOST PROTO((double *cost, int v0, int v1, int vertnum));
int ICOST PROTO((distances *dist, int v0, int v1));
int MCOST PROTO((distances *dist, int v0, int v1, int *lamda));
int TCOST PROTO((distances *dist, int v0, int v1, int *lamda, int mu));

#endif
