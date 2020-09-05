const User = require('../models/User');

module.exports = {
    async store( req, res ){
        const { email, token, password } = req.body

        try{
            const user = await User.findOne({ email }).select('+passwordResetToken passwordResetExpires')

            if(!user)
                return res.status(400).send({ error: 'Usuario nao encontrado' });

            if(token !== user.passwordResetToken)
                return res.status(400).send({ error: 'Codigo errado, tente gerar um novo' });

            const now = new Date();

            if(user.passwordResetExpires < now)
                return res.status(400).send({ error: 'O seu codigo expirou, por favor, gere um novo codigo' });

            user.password = password;
            await user.save();
            return res.send({ msg: 'A sua palavra passe foi alterada com sucesso!' });
        }catch( err ) {
            console.log('\nErro'+err);
            return res.status(400).send({ error: 'Ocorreu um erro' });
        }
    }
}