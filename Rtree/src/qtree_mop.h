/*
 * qtree_mop.h
 *
 *  
 *      Author: rever
 */

#ifndef QTREE_MOP_H_
#define QTREE_MOP_H_

namespace qtree
{

bool qtree_is_pareto(qtree *tree, qtree_point *point);

int qtree_count_dominating(qtree *tree, qtree_point *point);

}

#endif /* QTREE_MOP_H_ */
