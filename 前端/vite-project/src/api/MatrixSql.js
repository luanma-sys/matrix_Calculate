import axios from 'axios'
import { mapState } from 'vuex'
import useStore from '../store/index.js'

export default {
    getMat(matname){
        var str = useStore.getters.getGetMATURL
        return axios({
            method: 'get',
            url: str,
            params:{
                matname: matname
            }
        })
    },
    getAllMat(){
        var str = useStore.getters.getAllMATURL
        return axios({
            method: 'get',
            url: str
        })
    },
    saveMat(param){
        var str = useStore.getters.getSaveMATURL
        return axios({
            method: 'get',
            url: str,
            params: param
        })
    },

    
}