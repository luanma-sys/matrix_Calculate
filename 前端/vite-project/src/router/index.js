import { createRouter, createWebHistory } from 'vue-router'

import index from '../views/index.vue'
import List from '../views/Goods/Goodlist.vue'
import Cate from '../views/Goods/Category.vue'
import Goods from '../views/Goods/Home.vue'

import Basis from '../views/BasisTool.vue'

import { Dynamical,Differential,DynExample } from '../views/Dynamical'
import { DataTrend,LinearModel } from '../views/Orthogonality'
import { MatAdd,MatMul } from '../views/Basic'
import { history } from '../views/History'


const routes = [
    {
        path: '/basis',
        component: Basis,
        children: [
            {
                path: 'matadd',
                name: 'matadd',
                component: MatAdd
            },
            {
                path: 'matmul',
                name: 'matmul',
                component: MatMul
            }
        ]

    },
    {
        path: '/dyn',

        children: [
            {
                path: 'dyn',
                name: 'dyn',
                component: Dynamical
            },
            {
                path: 'diff',
                name: 'diff',
                component: Differential
            },
            {
                path: 'example',
                name: 'example',
                component: DynExample
            }
        ]
    },
    {
        path:'/orthogonality',
        children:[       
            {
                path: 'datatrend',
                name: 'datatrend',
                component: DataTrend
            },
            {
                path: 'linearmodel',
                name: 'linearmodel',
                component: LinearModel
            }
        ]
    },
    {
        path:'/singular',
        children:[       
            {
                path: 'list',
                name: 'list',
                component: List
            },
            {
                path: 'category',
                name: 'category',
                component: Cate
            }
        ]
    },
    {
        path:'/save',
        children:[
            {
                path: 'history',
                name: 'history',
                component: history
            }
        ]
    },
    {
        path: '/',
        component: index
    }
];

const router = createRouter({
    history: createWebHistory(),
    routes,

})

export default router;