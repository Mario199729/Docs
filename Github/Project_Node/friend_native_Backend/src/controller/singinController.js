const User = require('../models/User');
const bcrypt = require('bcryptjs');

module.exports = {
    async store( req, res ){
        const { email, password } = req.body;

        try {
            const user = await User.findOne({ email }).select('+password');
            if(!user)
                return res.status(400).send({ error: "Usuario nao encontrado, verifique se o e-mail foi informado correctamente." })
            
            if(!await bcrypt.compare(password, user.password))
                return res.status(400).send({ error: 'Password incorrecta, tente novamente, por favor' });

            user.password = undefined;
            return res.send({ user });
        } catch( err ) {
            return res.status(400).send({ error: "Ocorreu um erro, tente novamente" });
        }
    } 
}