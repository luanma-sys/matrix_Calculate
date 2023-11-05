import { toRef } from 'vue'
import { createStore } from 'vuex'

export default createStore({
    state: {
        baseMATURL: '/api/?interface=matrix',
        saveMATURL: '/sql/?intername=savematstorage',
        getMATURL: '/sql/?intername=getmatstorage',
        getAllMATURL: '/sql/?intername=getAllMat',
        isCollapse: false,
        matMessageSuspend: false,
        matevent: true,
      //  matMessage:
       // {
        id: 0,
        row: 0,
        col: 0,
        matrow: 0,
        matcol: 0,
      //  matrix: [],
      //  },


    },
    getters: {
        getmatMessageSuspend(state) {
            return state.matMessageSuspend
        },
        getBaseMATURL(state){
            return state.baseMATURL
        },
        getSaveMATURL(state){
            return state.saveMATURL
        },
        getGetMATURL(state){
            return state.getMATURL
        },
        getAllMATURL(state){
            return state.getAllMATURL
        }
    }
    ,
    mutations: {
        changeIsCollapse(state) {
            state.isCollapse = !state.isCollapse
        },
        openMatMessageSuspend(state,[R,C]) {
            state.matMessageSuspend = true
            
            state.matrow = R
            state.matcol = C
            state.row = state.matrow
            state.col = state.matcol
        },
        closeMatMessageSuspend(state) {
            state.matMessageSuspend = false;
    

        },
        setRow(state,R)
        {
             state.row = R
        },
        setCol(state,C)
        {
            state.col = C
        },
        submitmat(state)
        {
            state.matrow = state.row
            setTimeout(function(){state.matcol = state.col }, 100)
        }
    }
})

