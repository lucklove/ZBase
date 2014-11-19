/**************************************************************************
 * sort.h                                                                 * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

/**
 * \brief Privide sort function to sort data.
 * \param data The data to be sort.
 * \param size Size of per item in data.
 * \param num Item num in data.
 * \param cmp Cmp function provide by user.
 */
void quickSort(void *data, unsigned int size, unsigned int num, int (*cmp)(const void *, const void *));
void stackSort(void *data, unsigned int size, unsigned int num, int (*cmp)(const void *, const void *));
