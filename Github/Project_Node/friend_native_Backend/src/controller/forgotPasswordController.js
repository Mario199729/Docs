const User = require('../models/User');
const crypto = require('crypto');
const mailer = require('../module/mailer');

module.exports = {
    async store( req, res ){

        const { email } = req.body;
        try{
            const user = await User.findOne({ email }).select('+password')

            if(!user)
                return res.status(401).send({ error: 'Usuario nao encotrado, verifique se o seu e-mail foi infromado correctamente!' });
            
            const token = crypto.randomBytes(10).toString('hex');

            const now = new Date();
            now.setHours(now.getHours() + 1);

            await User.findByIdAndUpdate(user.id, {
                '$set': {
                    passwordResetToken: token,
                    passwordResetExpires: now,
                }
            });

            mailer.sendMail({
                to: email,
                from: 'support.job-in@nativeJS.com',
                template: 'auth/forget',
                context: { token },
            }, ( err ) => {
                if(err){
                    console.log('\n Erro:'+err);
                    return res.status(400).send({ error: 'Cannot send forgot password email' });
                }
                return res.send();
            });

        } catch( err ){
            console.log('\nErro:... '+ err);
            return res.status(401).send({ error: 'houve um erro, tente novamente.' })
        }
        
    }
}