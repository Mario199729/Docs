const User = require('../models/User');

module.exports = {
    async store( req, res ){

        const { email } = req.body;
        try {
            if(await User.findOne({ email }))
                return res.status(400).send({ error: 'E-mail ja foi usado por um outro usuario, tente com outro e-mail' })
            const user = await User.create(req.body);

            user.password = undefined;
            res.send({ user });

        } catch( err ) {
            console.log('\n Erro: '+err);
            return res.status(400).send({ error: 'Houve um erro ao cadastrar, por favor, tente novamente.' })
        }
    }
}