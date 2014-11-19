/**************************************************************************
 * random.h                                                               * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

/** 
 * \brief Set a seed for random number generator.
 * \param seed The seed to set.
 */
void randSeed(unsigned int seed);

/**
 * \brief Get a interger random number.
 * \return A random interger number.
 */
unsigned int randGen(void);
