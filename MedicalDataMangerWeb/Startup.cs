using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(MedicalDataMangerWeb.Startup))]
namespace MedicalDataMangerWeb
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
        }
    }
}
